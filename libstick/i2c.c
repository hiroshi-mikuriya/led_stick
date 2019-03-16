#include "i2c.h"
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

static int s_fd = 0;
static uint8_t s_dev_addr;

int i2c_init(uint8_t dev_addr)
{
	i2c_deinit();
	const char * dev = "/dev/i2c-1";
	int res = open(dev, O_RDWR);
	if (res < 0) {
		fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
		return errno;
	}
	s_fd = res;
	s_dev_addr = dev_addr;
	if (ioctl(s_fd, I2C_SLAVE, s_dev_addr) < 0) {
		fprintf(stderr, "error ioctl(I2C_SLAVE) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}

int i2c_deinit(void)
{
	if (s_fd != 0 && close(s_fd) < 0) {
		fprintf(stderr, "error close %s\n", strerror(errno));
		return errno;
	}
	s_fd = 0;
	return 0;
}

int i2c_write(uint8_t * buf, uint16_t len)
{
	int res = write(s_fd, buf, len);
	if (res < 0) {
		fprintf(stderr, "error write %s\n", strerror(errno));
		return errno;
	}
	if (res != len) {
		fprintf(stderr, "error write size %d != %d\n", res, len);
		return -1;
	}
	return 0;
}

int i2c_read(uint8_t * buf, uint16_t len)
{
	int res = read(s_fd, buf, len);
	if (res < 0) {
		fprintf(stderr, "error read %s\n", strerror(errno));
		return errno;
	}
	if (res != len) {
		fprintf(stderr, "error read size %d != %d\n", res, len);
		return -1;
	}
	return 0;
}

int i2c_write_read(uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen)
{
	struct i2c_msg msgs[2] = {
		{ .addr = s_dev_addr, .flags = 0, /*write*/ .len = wlen, .buf = wbuf },
		{ .addr = s_dev_addr, .flags = I2C_M_RD, .len = rlen, .buf = rbuf }
	};  
	struct i2c_rdwr_ioctl_data rdwr = { .msgs = msgs, .nmsgs = 2 };
	if (ioctl(s_fd, I2C_RDWR, &rdwr) < 0) {
		fprintf(stderr, "error ioctl(I2C_RDWR) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}

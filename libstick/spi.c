#include "spi.h"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

static int s_fd = 0;

int spi_init(void)
{
	if (s_fd != 0 && close(s_fd) < 0) {
		fprintf(stderr, "error close %s\n", strerror(errno));
		return errno;
	}
	const char * dev = "/dev/spidev0.0";
	s_fd = open(dev, O_RDWR);
	if (s_fd < 0) {
		fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
		return errno;
	}
	return 0;
}

int spi_deinit(void)
{
	if (s_fd != 0 && close(s_fd) < 0) {
		fprintf(stderr, "error close %s\n", strerror(errno));
		return errno;
	}
	s_fd = 0;
	return 0;
}

int spi_write(uint8_t * buf, uint16_t len, int cs)
{
#define NUM_OF_MSG   1
	struct spi_ioc_transfer mesgs[NUM_OF_MSG] = {
		{
			.tx_buf = (unsigned long)buf,
			//.rx_buf,
			.len = len,
			.speed_hz = 2 * 1000 * 1000,
			//.delay_usecs
			.bits_per_word = 8,
			.cs_change = cs,
			//.tx_nbits,
			//.rx_nbits,
			//.pad,
		}
	};
	if (ioctl(s_fd, SPI_IOC_MESSAGE(NUM_OF_MSG), &mesgs) < 0) {
		fprintf(stderr, "error ioctl(SPI_IOC_MAGIC) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}

int spi_read(uint8_t * buf, uint16_t len, int cs)
{
	return -1;
}

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

static int s_fd;

int spi_init(void)
{
    const char * dev = "/dev/spidev0.1";
    s_fd = open(dev, O_RDWR);
    if (s_fd < 0) {
        fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
        return errno;
    }
    return 0;
}

int spi_deinit(void)
{
    if (close(s_fd) < 0) {
        fprintf(stderr, "error close %s\n", strerror(errno));
        return errno;
    }
    return 0;
}

int spi_write(uint8_t * buf, uint16_t len, int cs)
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

int spi_read(uint8_t * buf, uint16_t len, int cs)
{
    if (read(s_fd, buf, len) < 0) {
        fprintf(stderr, "error read %s\n", strerror(errno));
        return errno;
    }
    return 0;
}

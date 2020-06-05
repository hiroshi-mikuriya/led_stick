#include "spi.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int s_fd = 0;

int spi_init(void) {
  int res = 0;
  if (s_fd) return res;
  const char *dev = "/dev/spidev0.0";
  int fd = open(dev, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
    return errno;
  }
  s_fd = fd;
  uint8_t mode = 0, bitjust = 0, word = 0;
  uint32_t speed = 0, mode32 = 0;
  puts("SPI settings");
  if (ioctl(s_fd, SPI_IOC_RD_MODE, &mode) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_RD_MODE) %s\n", strerror(errno));
    res = errno;
    goto error;
  }
  printf("Mode : %u\n", mode);
  if (ioctl(s_fd, SPI_IOC_RD_LSB_FIRST, &bitjust) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_RD_LSB_FIRST) %s\n", strerror(errno));
    res = errno;
    goto error;
  }
  printf("LSB First : %u\n", bitjust);
  if (ioctl(s_fd, SPI_IOC_RD_BITS_PER_WORD, &word) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_RD_BITS_PER_WORD) %s\n",
            strerror(errno));
    res = errno;
    goto error;
  }
  printf("BitsPerWord : %u\n", word);
  if (ioctl(s_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_RD_MAX_SPEED_HZ) %s\n",
            strerror(errno));
    res = errno;
    goto error;
  }
  printf("MaxSpeedHz : %u\n", speed);
  if (ioctl(s_fd, SPI_IOC_RD_MODE32, &mode32) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_RD_MODE32) %s\n", strerror(errno));
    res = errno;
    goto error;
  }
  printf("ModeField : %u\n", mode32);
  return res;
error:
  spi_deinit();
  return errno;
}

int spi_deinit(void) {
  if (s_fd != 0 && close(s_fd) < 0) {
    fprintf(stderr, "error close %s\n", strerror(errno));
    return errno;
  }
  s_fd = 0;
  return 0;
}

int spi_write(uint8_t *buf, uint16_t len, int cs) {
#define NUM_OF_MSG 1
  struct spi_ioc_transfer mesgs[NUM_OF_MSG] = {{
      .tx_buf = (unsigned long)buf,
      .len = len,
      .speed_hz = 2 * 1000 * 1000,
      .bits_per_word = 8,
      .cs_change = cs,
  }};
  if (ioctl(s_fd, SPI_IOC_MESSAGE(NUM_OF_MSG), &mesgs) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
    return errno;
  }
  return 0;
#undef NUM_OF_MSG
}

int spi_read(uint8_t *buf, uint16_t len, int cs) {
#define NUM_OF_MSG 1
  struct spi_ioc_transfer mesgs[NUM_OF_MSG] = {{
      .rx_buf = (unsigned long)buf,
      .len = len,
      .speed_hz = 2 * 1000 * 1000,
      .bits_per_word = 8,
      .cs_change = cs,
  }};
  if (ioctl(s_fd, SPI_IOC_MESSAGE(NUM_OF_MSG), &mesgs) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
    return errno;
  }
  return 0;
#undef NUM_OF_MSG
}

int spi_write_read(uint8_t *wbuf, uint16_t wlen, uint8_t *rbuf, uint16_t rlen,
                   int cs) {
  const uint32_t speed_hz = 2 * 1000 * 1000;
  const uint8_t bits_per_word = 8;
#define NUM_OF_MSG 2
  struct spi_ioc_transfer mesgs[NUM_OF_MSG] = {
      {
          .tx_buf = (unsigned long)wbuf,
          .len = wlen,
          .speed_hz = speed_hz,
          .bits_per_word = bits_per_word,
          .cs_change = cs,
      },
      {
          .rx_buf = (unsigned long)rbuf,
          .len = rlen,
          .speed_hz = speed_hz,
          .bits_per_word = bits_per_word,
          .cs_change = cs,
      }};
  if (ioctl(s_fd, SPI_IOC_MESSAGE(NUM_OF_MSG), &mesgs) < 0) {
    fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
    return errno;
  }
  return 0;
#undef NUM_OF_MSG
}
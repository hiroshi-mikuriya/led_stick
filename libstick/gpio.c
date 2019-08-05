// 参考URL: https://www.ei.tohoku.ac.jp/xkozima/lab/raspTutorial3.html

#include "gpio.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define BLOCK_SIZE 4096
#define REG_BASE 0x3F000000
#define GPIO_BASE (REG_BASE + 0x200000)

#define GPSEL0 0
#define GPSET0 7
#define GPCLR0 10
#define GPLEV0 13
#define GPPUD 37
#define GPPUDCLK0 38

static volatile uint32_t *s_gpio_base = NULL;

int gpio_init(void) {
  gpio_deinit();
  const char *dev = "/dev/gpiomem";
  int fd = open(dev, O_RDWR | O_SYNC);
  if (fd < 0) {
    fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
    return errno;
  }
  void *gpio_mmap =
      mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
  if (gpio_mmap == MAP_FAILED) {
    fprintf(stderr, "error mmap %s\n", strerror(errno));
    return errno;
  }
  if (close(fd) < 0) {
    fprintf(stderr, "error close %s\n", strerror(errno));
    return errno;
  }
  s_gpio_base = (volatile uint32_t *)gpio_mmap;
  return 0;
}

int gpio_deinit(void) {
  if (s_gpio_base != NULL && munmap((void *)s_gpio_base, BLOCK_SIZE) < 0) {
    fprintf(stderr, "error munmap %s\n", strerror(errno));
    return errno;
  }
  s_gpio_base = NULL;
  return 0;
}

void gpio_configure(int pin, int mode) {
  int index = pin / 10;
  uint32_t shift = (pin % 10) * 3;
  uint32_t mask = ~(0b0111 << shift);
  s_gpio_base[index] = (s_gpio_base[index] & mask) | ((mode & 0b0111) << shift);
}

void gpio_set_pull(int pin, int pull) {
  s_gpio_base[GPPUD] = pull & 0b0011;
  usleep(1);
  s_gpio_base[GPPUDCLK0] = 1 << pin;  // ピンにクロックを供給
  usleep(1);
  // プルモード・クロック状態をクリア
  s_gpio_base[GPPUD] = 0;
  s_gpio_base[GPPUDCLK0] = 0;
}

void gpio_write(uint32_t pin, uint32_t pol) {
  int reg = (pol == 0) ? GPCLR0 : GPSET0;
  s_gpio_base[reg] |= 1 << pin;
}

uint32_t gpio_read(uint32_t pin) {
  return (s_gpio_base[GPLEV0] & (1 << pin)) != 0;
}

// 参考URL: https://www.ei.tohoku.ac.jp/xkozima/lab/raspTutorial3.html

#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint-gcc.h>

#define BLOCK_SIZE		4096
#define REG_BASE		0x3F200000
#define GPIO_BASE		(REG_BASE+0x200000)

#define GPIO_INPUT		0
#define GPIO_OUTPUT		1

static volatile uint32_t * s_gpio_base = NULL;

int gpio_init(void)
{
	gpio_deinit();
	const char * dev = "/dev/gpiomem";
	int fd = open(dev, O_RDWR | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
		return errno;
	}
	void * gpio_mmap = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
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

int gpio_deinit(void)
{
	if (s_gpio_base != NULL && munmap((void *)s_gpio_base, BLOCK_SIZE) < 0) {
		fprintf(stderr, "error munmap %s\n", strerror(errno));
		return errno;
	}
	s_gpio_base = NULL;
	return 0;
}

static void configure(int pin, int mode)
{
    int index = pin / 10;
    unsigned int mask = ~(7 << ((pin % 10) * 3));
    s_gpio_base[index] = (s_gpio_base[index] & mask) | ((mode & 7) << ((pin % 10) * 3));
}

void gpio_write(uint32_t pin, uint32_t pol)
{
	configure(pin, GPIO_OUTPUT);
	if (pol == 0) {
		s_gpio_base[10] |= 1 << pin;
	} else {
		s_gpio_base[7] |= 1 << pin;
	}
}

uint32_t gpio_read(uint32_t pin)
{
	configure(pin, GPIO_INPUT);
	return (s_gpio_base[13] & (1 << pin)) != 0;
}

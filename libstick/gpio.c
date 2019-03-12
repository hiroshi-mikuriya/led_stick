#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint-gcc.h>

#define BLOCK_SIZE 4096
#define REG_BASE 0x3F200000
#define GPIO_BASE (REG_BASE+0x200000)
#define FSEL0_INDEX 0
#define FSEL2_INDEX 0x02
#define SET0_INDEX 0x07
#define CLR0_INDEX 0x0A
#define GPF_OUTPUT 0x01

static volatile uint32_t * s_gpio_base = NULL;

static void function_set(int pin, uint32_t func)
{
	int index = FSEL2_INDEX;
	uint32_t mask = ~(0x7 << ((pin % 10) * 3));
	uint32_t masked = (s_gpio_base[index] & mask);
	uint32_t added = (func & 0x7) << ((pin % 10) * 3);
	uint32_t final = masked | added;
	s_gpio_base[index] = final;
}

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

void gpio_write(uint32_t pin, uint32_t pol)
{
	// TODO: 実装する
}

uint32_t gpio_read(uint32_t pin)
{
	//function_set(pin, 0);
	for (int i = 0; i < 16; ++i) {
		printf("%2d: %08X\n", i, s_gpio_base[i]);
	}
	puts("");
	return 0;
}

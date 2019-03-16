// 参考URL: http://masquaremo.github.io/blog/2013/01/29/223448/

#include "pwm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint-gcc.h>

#define REG_BASE		0x3F000000
#define PWM_BASE		(REG_BASE + 0x20C000)
#define CLOCK_BASE		(REG_BASE + 0x101000)

// PWM controller offset
#define    PWM_CONTROL 0
#define	PWM_STATUS  1
#define	PWM0_RANGE  4
#define	PWM0_DATA   5
#define	PWM1_RANGE  8
#define	PWM1_DATA   9

// GPIO clock reg offset
#define PWMCLK_CNTL 40
#define PWMCLK_DIV  41

// BCM Magic
#define	BCM_PASSWORD 0x5A000000

#define	PWM1_MS_MODE    0x8000  // Run in MS mode
#define	PWM1_USEFIFO    0x2000  // Data from FIFO
#define	PWM1_REVPOLAR   0x1000  // Reverse polarity
#define	PWM1_OFFSTATE   0x0800  // Ouput Off state
#define	PWM1_REPEATFF   0x0400  // Repeat last value if FIFO empty
#define	PWM1_SERIAL     0x0200  // Run in serial mode
#define	PWM1_ENABLE     0x0100  // Channel Enable

#define	PWM0_MS_MODE    0x0080  // Run in MS mode
#define	PWM0_USEFIFO    0x0020  // Data from FIFO
#define	PWM0_REVPOLAR   0x0010  // Reverse polarity
#define	PWM0_OFFSTATE   0x0008  // Ouput Off state
#define	PWM0_REPEATFF   0x0004  // Repeat last value if FIFO empty
#define	PWM0_SERIAL     0x0002  // Run in serial mode
#define	PWM0_ENABLE     0x0001  // Channel Enable

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

static volatile uint32_t * s_pwm_base = NULL;
static volatile uint32_t * s_clk_base = NULL;

int pwm_init(void)
{
	pwm_deinit();
	const char * dev = "/dev/gpiomem";
	int fd = open(dev, O_RDWR | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
		return errno;
	}
	void * pwm_mmap = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PWM_BASE);
	if (pwm_mmap == MAP_FAILED) {
		fprintf(stderr, "error mmap(pwm) %s\n", strerror(errno));
		return errno;
	}
	void * clk_mmap = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CLOCK_BASE);
	if (clk_mmap == MAP_FAILED) {
		fprintf(stderr, "error mmap(clk) %s\n", strerror(errno));
		return errno;
	}
	if (close(fd) < 0) {
		fprintf(stderr, "error close %s\n", strerror(errno));
		return errno;
	}
	s_pwm_base = (volatile uint32_t *)pwm_mmap;
	s_clk_base = (volatile uint32_t *)clk_mmap;
	*(s_pwm_base + PWM_CONTROL) = 0; // Stop PWM
	*(s_clk_base + PWMCLK_CNTL) = BCM_PASSWORD | 0x01; // Stop PWM Clock
	usleep(110000); // See comments in pwmSetClockWPi
    while (*(s_clk_base + PWMCLK_CNTL) & 0x80) // Wait for clock to be !BUSY
        usleep(1000) ;
	for (int channel = 0; channel <= 1; ++channel) {
		pwm_set_range(channel, 1024);
		usleep(10000);
		pwm_set_data(channel, 0);
		usleep(10000);
	}
	return 0;
}

int pwm_deinit(void)
{
	if (s_pwm_base != NULL && munmap((void *)s_pwm_base, BLOCK_SIZE) < 0) {
		fprintf(stderr, "error munmap(pwm) %s\n", strerror(errno));
		return errno;
	}
	if (s_clk_base != NULL && munmap((void *)s_clk_base, BLOCK_SIZE) < 0) {
		fprintf(stderr, "error munmap(clk) %s\n", strerror(errno));
		return errno;
	}
	s_pwm_base = NULL;
	s_clk_base = NULL;
	return 0;
}

void pwm_set_clock(uint32_t div)
{
	*(s_clk_base + PWMCLK_DIV)  = BCM_PASSWORD | (div << 12);
	*(s_clk_base + PWMCLK_CNTL) = BCM_PASSWORD | 0x11; // enable clk
}

void pwm_set_mode(uint8_t channel, uint8_t markspace, uint8_t enabled)
{
	int flag = (channel == 0)? PWM0_ENABLE : PWM1_ENABLE;
	if (enabled)
		*(s_pwm_base + PWM_CONTROL) |= flag;
	else
		*(s_pwm_base + PWM_CONTROL) &= ~flag;
}

void pwm_set_range(uint8_t channel, uint32_t range)
{
	int pos = (channel == 0)? PWM0_RANGE : PWM1_RANGE;
	*(s_pwm_base + pos) = range;
	usleep(1000);
}

void pwm_set_data(uint8_t channel, uint32_t data)
{
	int pos = (channel == 0)? PWM0_DATA : PWM1_DATA;
	*(s_pwm_base + pos) = data;
	usleep(1000);
}

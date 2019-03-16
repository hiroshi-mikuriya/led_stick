#pragma once

#include <stdint.h>

// mode定義
#define GPIO_INPUT		0
#define GPIO_OUTPUT		1
#define GPIO_ALT0		4
#define GPIO_ALT1		5
#define GPIO_ALT2		6
#define GPIO_ALT3		7
#define GPIO_ALT4		3
#define GPIO_ALT5		2

/**
 * @return 0:成功 0以外:失敗
 */
int gpio_init(void);

/**
 * @return 0:成功 0以外:失敗
 */
int gpio_deinit(void);

/**
 * @param [in] pin
 * @param [in] mode
 */
void gpio_configure(int pin, int mode);

/**
 * @param [in] pin
 * @param [in] pol
 */
void gpio_write(uint32_t pin, uint32_t pol);

/**
 * @param [in] pin
 * @return pol
 */
uint32_t gpio_read(uint32_t pin);

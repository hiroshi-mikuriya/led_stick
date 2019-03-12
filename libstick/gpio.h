#pragma once

#include <stdint.h>

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
 * @param [in] pol
 */
void gpio_write(uint32_t pin, uint32_t pol);

/**
 * @param [in] pin
 * @return pol
 */
uint32_t gpio_read(uint32_t pin);

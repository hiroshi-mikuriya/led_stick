#pragma once

#include <stdint.h>

/**
 * @return 0:成功 0以外:失敗
 */
int pwm_init(void);

/**
 * @return 0:成功 0以外:失敗
 */
int pwm_deinit(void);

/**
 * @param[in] div (19.2 / div MHz)
 */
void pwm_set_clock(uint32_t div);

/**
 * @param[in] channel The PWM channel. 0 or 1.
 * @param[in] markspace Set true if you want Mark-Space mode. 0 for Balanced mode.
 * @param[in] enabled Set true to enable this channel and produce PWM pulses.
 */
void pwm_set_mode(uint8_t channel, uint8_t markspace, uint8_t enabled);

/**
 * @param[in] channel The PWM channel. 0 or 1.
 * @param[in] range The maximum value permitted for DATA.
 */
void pwm_set_range(uint8_t channel, uint32_t range);

/**
 * @param[in] channel The PWM channel. 0 or 1.
 * @param[in] data Controls the PWM output ratio as a fraction of the range.
 *                 Can vary from 0 to RANGE.
 */
void pwm_set_data(uint8_t channel, uint32_t data);

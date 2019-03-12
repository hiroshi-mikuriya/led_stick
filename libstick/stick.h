#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @return 0:成功 0以外:失敗
 */
int stick_init(void);

/**
 * @return 0:成功 0以外:失敗
 */
int stick_deinit(void);

/**
 * @return 0:成功 0以外:失敗
 */
int stop_led_demo(void);

/**
 * @param[in] line 0 - 1364
 * @param[in] pattern LED flash pattern. It should has 32 * 3 items. RGB value should be between 0 and 3.
 * @return 0:成功 0以外:失敗
 */
int write_line(uint16_t line, uint8_t * pattern);

/**
 * @param[in] line 0 - 1364
 * @return 0:成功 0以外:失敗
 */
int show_line(uint16_t line);

/**
 * @param[out] accel [x, y, z]
 * @return 0:成功 0以外:失敗
 */
int get_accel(short * accel);

/**
 * @param[out] gyro [x, y, z]
 * @return 0:成功 0以外:失敗
 */
int get_gyro(short * gyro);

/**
 * @param[out] button
 * @return 0:成功 0以外:失敗
 */
int get_button(uint8_t * button);

#ifdef __cplusplus
}
#endif /* __cplusplus */

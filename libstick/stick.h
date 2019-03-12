#pragma once

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

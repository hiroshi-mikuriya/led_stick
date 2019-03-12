#pragma once

#include <stdint.h>

/**
 * @param [in] dev_addr
 * @return 0:成功 0以外:失敗
 */
int i2c_init(uint8_t dev_addr);

/**
 * @return 0:成功 0以外:失敗
 */
int i2c_deinit(void);

/**
 * @param [in] buf
 * @param [in] len
 * @return 0:成功 0以外:失敗
 */
int i2c_write(uint8_t * buf, uint16_t len);

/**
 * @param [out] rbuf
 * @param [in] rlen
 * @return 0:成功 0以外:失敗
 */
int i2c_read(uint8_t * buf, uint16_t len);

/**
 * @param [in] wbuf
 * @param [in] wlen
 * @param [out] rbuf
 * @param [in] rlen
 * @return 0:成功 0以外:失敗
 */
int i2c_write_read(uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen);

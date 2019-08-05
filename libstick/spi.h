#pragma once

#include <stdint.h>

/**
 * @return 0:成功 0以外:失敗
 */
int spi_init(void);

/**
 * @return 0:成功 0以外:失敗
 */
int spi_deinit(void);

/**
 * @param [in] buf
 * @param [in] len
 * @param [in] cs chip select 0 or 1
 * @return 0:成功 0以外:失敗
 */
int spi_write(uint8_t* buf, uint16_t len, int cs);

/**
 * @param [in] buf
 * @param [in] len
 * @param [in] cs chip select 0 or 1
 * @return 0:成功 0以外:失敗
 */
int spi_read(uint8_t* buf, uint16_t len, int cs);

/**
 * @param [in] wbuf
 * @param [in] wlen
 * @param [in] rbuf
 * @param [in] rlen
 * @param [in] cs chip select 0 or 1
 * @return 0:成功 0以外:失敗
 */
int spi_write_read(uint8_t* wbuf, uint16_t wlen, uint8_t* rbuf, uint16_t rlen,
                   int cs);
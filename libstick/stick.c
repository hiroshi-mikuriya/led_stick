#include "stick.h"
#include "i2c.h"
#include "spi.h"
#include "mpu6050reg.h"

#define MPU6050_SLAVE   0x68
#define SPI_WRITE	   0x02

int stick_init(void)
{
	int res = 0;
	res = i2c_init(MPU6050_SLAVE);
	if (res != 0)
		goto error;
	res = spi_init();
	if (res != 0)
		goto error;
	uint8_t v[] = { PWR_MGMT_1, 0 };
	res = i2c_write(v, sizeof(v));
	if (res != 0)
		goto error;
	return 0;
error:
	i2c_deinit();
	spi_deinit();
	return res;
}

int stick_deinit(void)
{
	int res0 = i2c_deinit();
	int res1 = spi_deinit();
	if (res0 != 0)
		return res0;
	if (res1 != 0)
		return res1;
	return 0;
}

int stop_led_demo(void)
{
	uint8_t d[] = { SPI_WRITE, 0, 2, 0, 0 }; // clear
	return spi_write(d, sizeof(d), 0);
}

int write_line(uint16_t line, uint8_t * pattern)
{
	int addr = 0x8000 + line * 24;
	uint8_t d[24 + 3] = { SPI_WRITE, (addr >> 8) & 0xFF, addr & 0xFF };
	static const int s_table[] = { 20, 18, 16, 10, 8, 22, 0, 14, 12, 6, 4, 2 };
	for (int led = 0; led < 32; ++led) {
		for (int rgb = 0; rgb < 3; ++rgb) {
			char v = pattern[led * 3 + rgb] & 0x03;
			int n = s_table[(led % 4) * 3 + rgb] + (led / 4) * 24;
			int i0 = n / 8;
			int i1 = n % 8;
			d[i0 + 3] |= v << i1;
		}
	}
	return spi_write(d, sizeof(d), 0);
}

int show_line(uint16_t line)
{
	uint8_t d[] = { SPI_WRITE, 0, 0, (line >> 8) & 0xFF, line & 0xFF };
	return spi_write(d, sizeof(d), 0);
}

int get_accel(short * accel)
{
	uint8_t v = ACCEL_XOUT_H;
	uint8_t buf[6] = { 0 };
	int res = i2c_write_read(&v, sizeof(v), buf, sizeof(buf));
	if (res != 0)
		return res;
	for (int i = 0; i < 3; ++i) {
		accel[i] = (short)((buf[i * 2] << 8) + (buf[i * 2 + 1] & 0xFF));
	}
	return 0;
}

int get_gyro(short * gyro)
{
	uint8_t v = GYRO_XOUT_H;
	uint8_t buf[6] = { 0 };
	int res = i2c_write_read(&v, sizeof(v), buf, sizeof(buf));
	if (res != 0)
		return res;
	for (int i = 0; i < 3; ++i) {
		gyro[i] = (short)((buf[i * 2] << 8) + (buf[i * 2 + 1] & 0xFF));
	}
	return 0;
}

int get_button(uint8_t * button)
{
	return -1;
}

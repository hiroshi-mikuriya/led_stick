#include "mpu6050.h"

#include <unistd.h>  // usleep

#include "../i2c.h"
#include "../mpu6050reg.h"

static uint8_t s_dev_addr;  // slave address of mpu6050 expected 0x68 o r0x69.

int init_mpu6050(uint8_t dev_addr) {
  s_dev_addr = dev_addr;
  int err = 0;
#define CHK(proc)      \
  do {                 \
    err = proc;        \
    if (err) goto end; \
  } while (0)
  CHK(i2c_init(s_dev_addr));
  uint8_t v[] = {PWR_MGMT_1, 0};
  CHK(i2c_write(v, sizeof(v)));
#undef CHK
end:
  i2c_deinit();
  return err;
}

int read_accel(short *accel) {
  int err = 0;
#define CHK(proc)      \
  do {                 \
    err = proc;        \
    if (err) goto end; \
  } while (0)
  CHK(i2c_init(s_dev_addr));
  uint8_t v = ACCEL_XOUT_H;
  uint8_t buf[6] = {0};
  // CHK(i2c_write(&v, sizeof(v)));
  // CHK(i2c_read(buf, sizeof(buf)));
  CHK(i2c_write_read(&v, sizeof(v), buf, sizeof(buf)));
  for (int i = 0; i < 3; ++i) {
    accel[i] = (short)((buf[i * 2] << 8) + (buf[i * 2 + 1] & 0xFF));
  }
#undef CHK
end:
  i2c_deinit();
  return err;
}

int read_gyro(short *gyro) {
  int err = 0;
#define CHK(proc)      \
  do {                 \
    err = proc;        \
    if (err) goto end; \
  } while (0)
  CHK(i2c_init(s_dev_addr));
  uint8_t v = GYRO_XOUT_H;
  uint8_t buf[6] = {0};
  // CHK(i2c_write(&v, sizeof(v)));
  // CHK(i2c_read(buf, sizeof(buf)));
  CHK(i2c_write_read(&v, sizeof(v), buf, sizeof(buf)));
  for (int i = 0; i < 3; ++i) {
    gyro[i] = (short)((buf[i * 2] << 8) + (buf[i * 2 + 1] & 0xFF));
  }
#undef CHK
end:
  i2c_deinit();
  return err;
}

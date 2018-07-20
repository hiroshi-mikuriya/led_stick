# frozen_string_literal: true

REG_PWR_MGMT_1      = 0x6B
REG_INT_PIN_CFG     = 0x37
REG_GYRO_CONFIG     = 0x1B
REG_ACCEL_CONFIG1   = 0x1C
REG_ACCEL_CONFIG2   = 0x1D

MAG_MODE_POWERDOWN  = 0         # 磁気センサpower down
MAG_MODE_SERIAL_1   = 1         # 磁気センサ8Hz連続測定モード
MAG_MODE_SERIAL_2   = 2         # 磁気センサ100Hz連続測定モード
MAG_MODE_SINGLE     = 3         # 磁気センサ単発測定モード
MAG_MODE_EX_TRIGER  = 4         # 磁気センサ外部トリガ測定モード
MAG_MODE_SELF_TEST  = 5         # 磁気センサセルフテストモード

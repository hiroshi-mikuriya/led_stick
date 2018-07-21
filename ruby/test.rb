# frozen_string_literal: true

require './bcm2835'
require './def'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

def write(slave, cmd, d)
  BCM.bcm2835_i2c_begin
  BCM.bcm2835_i2c_setSlaveAddress(slave)
  BCM.bcm2835_i2c_setClockDivider(2500)
  d0 = [cmd, d].flatten.pack('C*')
  BCM.bcm2835_i2c_write(d0, d0.size)
  BCM.bcm2835_i2c_end
  sleep(0.0001)
end

def read(slave, cmd, size)
  BCM.bcm2835_i2c_begin
  BCM.bcm2835_i2c_setSlaveAddress(slave)
  BCM.bcm2835_i2c_setClockDivider(2500)
  d0 = [cmd].pack('C*')
  BCM.bcm2835_i2c_write(d0, d0.size)
  buf = ([0] * size).pack('C*')
  BCM.bcm2835_i2c_read(buf, buf.size)
  BCM.bcm2835_i2c_end
  buf
end

write(0x68, REG_PWR_MGMT_1, 0x80) # reset register
write(0x68, REG_PWR_MGMT_1, 0x00) # clear power management
write(0x68, REG_INT_PIN_CFG, 0x02) # enable AK8963 using I2C
write(0x68, REG_ACCEL_CONFIG1, 0x08) # mod acceleration sensor range.

loop do
  buf = read(0x68, 0x3B, 14)
  g = buf.unpack('s>*').map { |a| a * 8.0 / 0x8000 }
  p g
  sleep(0.1)
end

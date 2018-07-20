# frozen_string_literal: true

require './bcm2835'
require './def'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

BCM.bcm2835_i2c_begin
BCM.bcm2835_i2c_setSlaveAddress(0x68)
BCM.bcm2835_i2c_setClockDivider(2500)

def write(cmd, d)
  d0 = [cmd, d].flatten.pack('C*')
  BCM.bcm2835_i2c_write(d0, d0.size)
  sleep(0.0001)
end

write(REG_PWR_MGMT_1, 0x80) # reset register
write(REG_PWR_MGMT_1, 0x00) # clear power management
write(REG_INT_PIN_CFG, 0x02) # enable AK8963 using I2C
write(REG_ACCEL_CONFIG1, 0x08) # mod acceleration sensor range.

loop do
  write(0x3B, [])
  buf = ([0] * 14).pack('C*')
  BCM.bcm2835_i2c_read(buf, buf.size)
  g = buf.unpack('s>*').map { |a| a * 8.0 / 0x8000 }
  p g
  # puts(g: { x: g[0], y: g[1], z: g[2] }, angle: { x: angle[4], y: angle[5], z: angle[6]})
  sleep(0.1)
end

# BCM.bcm2835_i2c_end
# BCM.bcm2835_close

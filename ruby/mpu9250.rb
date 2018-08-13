# register map : https://cdn.sparkfun.com/assets/learn_tutorials/5/5/0/MPU-9250-Register-Map.pdf

require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

BCM.bcm2835_spi_begin
BCM.bcm2835_spi_setBitOrder(1) # MSB First
BCM.bcm2835_spi_setDataMode(0) # CPOL = 0, CPHA = 0
BCM.bcm2835_spi_setClockDivider(256) # 256 = 1.024us = 976.5625kHz

def write(addr, ary, cs)
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  a = [addr, ary].flatten.pack('C*')
  BCM.bcm2835_spi_writenb(a, a.size)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
end

def read(addr, len, cs)
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  a = [addr | 0x80].pack('C*')
  BCM.bcm2835_spi_writenb(a, a.size)
  res = ([0] * len).pack('C*')
  BCM.bcm2835_spi_transfern(res, res.size)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
  res
end

def writeb(addr, data, cs)
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  BCM.bcm2835_spi_transfer(addr | 0x80)
  BCM.bcm2835_spi_transfer(data)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
end

def readb(addr, cs)
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  BCM.bcm2835_spi_transfer(addr | 0x80)
  res = BCM.bcm2835_spi_transfer(0x00)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
  res
end

cs = 0

p readb(0x75, cs)

writeb(0x6B, 0x00, cs) # reset register
writeb(0x6B, 0x80, cs) # clear power management
write(0x37, 0x02, cs) # enable AK8963 using I2C
write(0x1C, 0x08, cs) # mod acceleration sensor range.

loop do
  p Array.new(6) { |i| readb(0x3B + i, cs) }
  sleep(0.2)
end

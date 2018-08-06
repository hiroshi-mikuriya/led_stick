require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

def write(addr, ary, cs)
  BCM.bcm2835_spi_begin
  BCM.bcm2835_spi_setBitOrder(1) # MSB First
  BCM.bcm2835_spi_setDataMode(3) # CPOL = 1, CPHA = 1
  BCM.bcm2835_spi_setClockDivider(256) # 256 = 1.024us = 976.5625kHz
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  a = [addr | 0x80, ary].flatten.pack('C*')
  BCM.bcm2835_spi_writenb(a, a.size)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
  BCM.bcm2835_spi_end
end

def read(addr, len, cs)
  BCM.bcm2835_spi_begin
  BCM.bcm2835_spi_setBitOrder(1) # MSB First
  BCM.bcm2835_spi_setDataMode(3) # CPOL = 1, CPHA = 1
  BCM.bcm2835_spi_setClockDivider(256) # 256 = 1.024us = 976.5625kHz
  BCM.bcm2835_spi_chipSelect(cs)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 0)
  a = [addr | 0x80].flatten.pack('C*')
  BCM.bcm2835_spi_writenb(a, a.size)
  res = ([0] * len).pack('C*')
  tmp = ([0] * len).pack('C*')
  # BCM.bcm2835_spi_transfern(res, res.size)
  BCM.bcm2835_spi_transfernb(tmp, res, res.size)
  BCM.bcm2835_spi_setChipSelectPolarity(cs, 1)
  BCM.bcm2835_spi_end
  res
end

write(0x6B, [0x80], 1)
write(0x6B, [0x00], 1)

loop do
  p read(0x3B, 6, 1).unpack('s*')
  sleep(0.2)
end

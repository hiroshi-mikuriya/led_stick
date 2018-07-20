require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init BCM2835'
  exit(1)
end

# d = [2, 0x80, 0x00, [0xAA]*24].flatten.pack('C*')
# d = [2, 0x80, 0x00, [0] * 5,0xFF,0xFF, [0]*17].flatten.pack('C*')
d0 = [2, 0x80, 0x00, 0x00, 0x00, 0x30, [0] * 21].flatten.pack('C*')
d1 = [2, 0x80, 24, 0x00, 0x00, 0xC0, [0] * 21].flatten.pack('C*')
SPI.write(d0, 0)
SPI.write(d1, 0)

loop do
  SPI.write([2, 0, 0, 0, 0].pack('C*'), 0)
  sleep(1)
  SPI.write([2, 0, 0, 0, 1].pack('C*'), 0)
  sleep(1)
end



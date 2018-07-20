require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

def write(addr, data)
  cs = 0
  SPI.write([2, [addr].pack('s>*').unpack('C*'), data].flatten.pack('C*'), cs)
end

# stop demo
write(2, [0, 0])

write(0x8000, [0, 0, 0x30, [0] * 21])
write(0x8000 + 24, [0, 0, 0xC0, [0] * 21])

loop do
  write(0, [0, 0])
  sleep(1)
  write(0, [0, 1])
  sleep(1)
end

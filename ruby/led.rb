require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init BCM2835'
  exit(1)
end

d = [2, 0, 2, 0, 0].pack('C*')

SPI.write(d, 0)


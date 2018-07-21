require './bcm2835'

if BCM.bcm2835_init.zero?
  puts 'failed to init bcm2835'
  exit 1
end

def write(addr, data)
  cs = 0
  SPI.write([2, [addr].pack('s>*').unpack('C*'), data].flatten.pack('C*'), cs)
end

##
# write line to LED pattern
# @param line [Integer] 0 - 1364
# @param ptn [Hash] ex. { 0 => {r:0,g:2,b:3}, 2 => {r:1,g:0,b:3}, ...]
def write_pattern(line, ptn)
  d = [0] * 24
  table = [
    { r: 20, g: 18, b: 16 },
    { r: 10, g: 8, b: 22 },
    { r: 0, g: 14, b: 12 },
    { r: 6, g: 4, b: 2 }
  ].freeze
  ptn.each do |i, rgb|
    rgb.each do |k, v|
      a = table[i % 4][k.to_sym] + (i / 4) * 24
      i0 = a / 8
      i1 = a % 8
      d[i0] |= v << i1
    end
  end
  addr = 0x8000 + line * 24
  write(addr, d)
end

##
# show line ( = flash LED)
# @param line [Interger] 0 - 1364
def show(line)
  write(0, [line].pack('s>*').unpack('C*'))
end

# stop demo
write(2, [0, 0])

show(0)

loop do
  32.times do |n|
    %w[r g b].each do |c|
      puts(led: n, color: c)
      4.times do |pwr|
        write_pattern(0, n => { c => pwr })
        sleep(0.3)
      end
    end
  end
end

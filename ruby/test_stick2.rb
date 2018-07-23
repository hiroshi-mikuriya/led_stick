# frozen_string_literal: true

require './stick_sdk'

if STICK.init_sdk.zero?
  puts 'failed to init SDK'
  exit 1
end

STICK.stop_led_demo

loop do
  g = ([0] * 6).pack('C*')
  STICK.get_accel(g)
  g0 = g.unpack('s*')
  line = (g0[2] + 15000) / 20
  next unless (0...1364).cover? line
  STICK.show_line(line)
end

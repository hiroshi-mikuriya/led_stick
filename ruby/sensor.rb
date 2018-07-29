# frozen_string_literal: true

require './stick_sdk'

if STICK.init_sdk.zero?
  puts 'failed to init SDK'
  exit 1
end

loop do
  a = ([0] * 6).pack('C*')
  STICK.get_accel(a)
  a0 = a.unpack('s*')
  g = a.clone
  STICK.get_gyro(g)
  g0 = g.unpack('s*')
  p(accel: a0, gyro: g0)
  sleep(0.5)
end

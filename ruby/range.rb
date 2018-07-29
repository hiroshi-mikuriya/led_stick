# frozen_string_literal: true

require './stick_sdk'

if STICK.init_sdk.zero?
  puts 'failed to init SDK'
  exit 1
end

min = [0xFFFF] * 6
max = [-0xFFFF] * 6

loop do
  a = ([0] * 6).pack('C*')
  STICK.get_accel(a)
  a0 = a.unpack('s*')
  STICK.get_gyro(a)
  a1 = a.unpack('s*')
  c = [a0, a1].flatten
  min = Array.new(6) { |i| [min[i], c[i]].min }
  max = Array.new(6) { |i| [max[i], c[i]].max }
  p Array.new(6) { |i| [min[i], max[i]] }
  sleep(0.5)
end

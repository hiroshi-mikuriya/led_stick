# frozen_string_literal: true

require './stick_sdk'

if STICK.init_sdk.zero?
  puts 'failed to init SDK'
  exit 1
end

STICK.stop_led_demo

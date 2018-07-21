# frozen_string_literal: true

require 'fiddle/import'

module STICK
  extend Fiddle::Importer
  dlload './stick_sdk.so'
  extern 'int init_sdk(void)'
  extern 'void stop_led_demo(void)'
  extern 'void write_led_pattern(int line, char * pattern)'
  extern 'void show_line(int line)'
end

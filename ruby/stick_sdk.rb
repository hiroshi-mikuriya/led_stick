# frozen_string_literal: true

require 'fiddle/import'

module STICK
  extend Fiddle::Importer
  dlload '../lib/stick_sdk.so'
  extern 'int init_sdk(void)'
  extern 'void stop_led_demo(void)'
  extern 'void write_line(int line, char * pattern)'
  extern 'void show_line(int line)'
  extern 'void get_accel(short *)'
  extern 'void get_gyro(short *)'
end

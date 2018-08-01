#include "stick_sdk.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace led {
  int const width = 16;
  int const height = 32;
  int const depth = 8;
  int const rgb565 = (5 + 6 + 5) / 8;
  int const udp_packet_size = width * height * depth * rgb565;
  typedef boost::array<char, udp_packet_size * 2> udp_buf_type;

  void write(led::udp_buf_type const & u, std::mutex & mtx) {
    for (int x = 0; x < led::width; ++x){
      char ptn[led::height * 3] = { 0 };
      for (int y = 0; y < led::height; ++y){
        for (int z = 0; z < led::depth; ++z){
          int i = ((x * led::height + y) * led::depth + z) * led::rgb565;
          char r = (u[i] & 0xF8);
          char g = ((u[i] & 0x07) << 5) + ((u[i + 1] & 0xE0) >> 3);
          char b = (u[i + 1] & 0x1F) << 3;
          ptn[y * 3] = std::max<char>(ptn[y * 3], r / 64);
          ptn[y * 3 + 1] = std::max<char>(ptn[y * 3 + 1], g / 64);
          ptn[y * 3 + 2] = std::max<char>(ptn[y * 3 + 2], b / 64);
        }
      }
      mtx.lock();
      write_line(x, ptn);
      mtx.unlock();
    }
  }
}

int main(int argc, const char * argv[])
{
  static_cast<void>(argc); // unused
  static_cast<void>(argv); // unused
  if(!init_sdk()){
    std::cerr << "failed to init stick sdk." << std::endl;
    return 1;
  }
  stop_led_demo();
  std::mutex mtx;
  std::thread th([&mtx]{
    for(;;){
      short a[3] = { 0 };
      get_accel(a);
      int const line = (a[1] + 0x8000) * led::width / 0x10000;
      mtx.lock();
      show_line(led::width - 1 - line);
      mtx.unlock();
	  sleep(1);
    }
  });
  namespace asio = boost::asio;
  namespace ip = asio::ip;
  asio::io_service io_service;
  ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 9001));
  led::udp_buf_type udp_buf;
  for (;;){
    ip::udp::endpoint ep;
    boost::system::error_code er;
    size_t len = socket.receive_from(boost::asio::buffer(udp_buf), ep, 0, er);
    std::cout << len << std::endl;
    if (len != led::udp_packet_size) {
      std::cerr << "size error : " << len << "\n" << std::endl;
      continue;
    }
    led::write(udp_buf, mtx);
  }
  // unreachable code
  th.join();
  return 0;
}

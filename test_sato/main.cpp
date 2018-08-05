#include "stick_sdk.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>

#define LED_COUNT 32 * 2
#define CENTER_AREA_COUNT 10 * 2

int main(int argc, const char * argv[])
{
  if(!init_sdk()){
    std::cerr << "failed to init stick sdk." << std::endl;
    return 2;
  }
  stop_led_demo();
  std::thread th([]{
    for(;;){
      short a[3] = {0};
      get_accel(a);
      double th = std::atan2(a[1], a[0]);
      int deg = static_cast<int>(0 < th ? th / CV_PI * 180 : th / CV_PI * 180 + 360);
      // std::cout << "angle: " << deg << " theta: " << th << std::endl;
      sleep(100);
    }
  });
  for(int i = 0; i < LED_COUNT * 3; ++i){
    char a[LED_COUNT * 3] = { 0 };
    a[i] = 0x0F;
    write_line(i, a);
  }
  for(;;){
    for(int i = 0; i < LED_COUNT * 3; ++i){
      show_line(i);
      sleep(200);
    }
  }
  return 0;
}

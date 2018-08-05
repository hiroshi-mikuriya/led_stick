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
  show_line(0);
  for(;;){
    for(int led = 0; led < 4; ++led){
      for(int rgb = 0; rgb < 3; ++rgb){
        char a[LED_COUNT * 3] = {0};
        a[led * 3 + rgb] = 0x0F;
        write_line(0, a);
        std::cout << "LED : " << led << " RGB : " << rgb << std::endl;
        sleep(2000);
      }
    }
  }
  return 0;
}
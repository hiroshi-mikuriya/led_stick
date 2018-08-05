#include "stick_sdk.h"
#include <thread>

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
      short acc[3] = {0};
      get_accel(acc);
      double th = std::atan2(a[1], a[0]);
      int deg = static_cast<int>(0 < th ? th / CV_PI * 180 : th / CV_PI * 180 + 360);
      std::cout << "angle: " << deg << " theta: " << th << std::endl;
      sleep(100);
    }
  });
  show_line(0);
  for(;;){
    for(int led = 0; led < LED_COUNT; ++led){
      for(int rgb = 0; rgb < 3; ++rgb){
        char a[LED_COUNT * 3] = {0};
        a[led * 3 + rgb] = 0x0F;
        write_line(0, a);
        sleep(100);
      }
    }
  }
  return 0;
}

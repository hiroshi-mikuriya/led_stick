#include "stick_sdk.h"
#include <opencv2/opencv.hpp>
#include <thread>

namespace
{
  std::vector<std::string> const images{ "o.png", "me.png", "de.png", "to.png", "u.png" };

  int s_select = 0;
  
  bool s_button_event = false;
  
  int sampling_button(int count){
    int res = 0;
    for(int i = 0; i < count; ++i){
      res += get_button();
      sleep(2);
    }
    return res;
  }
  void button_monitor()
  {
    int const count = 10;
    while(count != sampling_button(count));
    while(0 != sampling_button(count));
    s_select = (s_select + 1) % images.size();
    s_button_event = true;
    std::cerr << "button pushed!" << std::endl;
  }

  
  void write(cv::Mat m, int lines)
  {
    cv::Mat img;
    cv::resize(m, img, cv::Size(lines, 32));
    for(int x = 0; x < img.cols; ++x){
      char a[32 * 3] = { 0 };
      for(int y = 0; y < img.rows; ++y){
        cv::Vec3b const bgr = img.at<cv::Vec3b>(y, x);
        a[y * 3] = bgr[2] / 64; // 0 - 255 => 0 - 3
        a[y * 3 + 1] = bgr[1] / 64;
        a[y * 3 + 2] = bgr[0] / 64;
      }
      write_line(x, a);
    }
  }

  void show(int lines)
  {
    s_button_event = false;
    while(!s_button_event){
      short a[3] = { 0 };
      get_accel(a);
      int line = (a[1] + 0x8000) * lines / 0x10000;
      show_line(line);
      sleep(1);
    }
  }
}

int main(int argc, const char * argv[])
{
  if(!init_sdk()){
    std::cerr << "failed to init stick sdk." << std::endl;
    return 2;
  }
  std::thread th([]{
    for(;;){
    button_monitor();
  }
  });
  stop_led_demo();
  for(;;){
    cv::Mat img = cv::imread("../images/" + images[s_select], 1);
    if(img.empty()){
        std::cerr << "failed to open image file." << std::endl;
      return 3;
    }
    cv::flip(img, img, 1);
    int const lines = 1364;
    std::cerr << "writing image..." << std::endl;
    write(img, lines);
    std::cerr << "complete!" << std::endl;
    show(lines);
  }
  return 0;
}

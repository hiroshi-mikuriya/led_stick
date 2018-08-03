#include "stick_sdk.h"
#include <cmath>
#include <opencv2/opencv.hpp>

namespace
{
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
    for(;;){
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
  for(;;){
    short a[3] = { 0 };
    get_accel(a);
    double th = std::atan2(a[1], a[0]);
    std::cout << a[0] << " , " << a[1] << " , " << a[2] << " , " << th << std::endl;
    sleep(1);
  }
  if(argc < 2){
    std::cerr << "input image file path." << std::endl;
    return 1;
  }
  stop_led_demo();
  cv::Mat img = cv::imread(argv[1], 1);
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
  return 0;
}

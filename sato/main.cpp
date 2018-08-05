#include "stick_sdk.h"
#include <opencv2/opencv.hpp>

#define LED_COUNT 32 * 2
#define CENTER_AREA_COUNT 10 * 2

namespace
{
  void write(cv::Mat m)
  {
    int const s = LED_COUNT + CENTER_AREA_COUNT;
    double const r = s * 1.0 / m.rows;
    int const w = static_cast<int>(m.cols * r);
    cv::resize(m, m, cv::Size(w, s));
    std::cout << m.size() << std::endl;
    for(int deg = 0; deg < 360; deg += 2){
      cv::Point const center(m.cols / 2, m.rows / 2);
      double scale = 1.0;
      cv::Mat affine = cv::getRotationMatrix2D(center, deg, scale);
      cv::Mat rot;
      cv::warpAffine(m, rot, affine, m.size(), cv::INTER_CUBIC);
      char buf[LED_COUNT * 3] = { 0 };
      for(int y = 0; y < LED_COUNT; ++y){
        int y0 = y;
        if(LED_COUNT / 2 <= y){
          y0 += CENTER_AREA_COUNT;
        }
        auto const bgr = rot.at<cv::Vec3b>(y0, rot.cols / 2);
        buf[y * 3] = bgr[2] / 16;
        buf[y * 3 + 1] = bgr[1] / 16;
        buf[y * 3 + 2] = bgr[0] / 16;
      }
      write_line(deg / 2, buf);
    }
  }

  void show()
  {
    for(;;){
      short a[3] = { 0 };
      get_accel(a);
      double th = std::atan2(a[1], a[0]);
      int angle = static_cast<int>(0 < th ? th / CV_PI * 180 : th / CV_PI * 180 + 360);
      show_line(angle / 2);
      sleep(1);
    }
  }
}

int main(int argc, const char * argv[])
{
  if(argc < 2){
    std::cerr << "input image file path." << std::endl;
    return 1;
  }
  if(!init_sdk()){
    std::cerr << "failed to init stick sdk." << std::endl;
    return 2;
  }
  stop_led_demo();
  cv::Mat img = cv::imread(argv[1], 1);
  if(img.empty()){
    std::cerr << "failed to open image file." << std::endl;
    return 3;
  }
  cv::flip(img, img, 1);
  std::cerr << "writing image..." << std::endl;
  write(img);
  std::cerr << "complete!" << std::endl;
  show();
  return 0;
}

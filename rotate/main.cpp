#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[])
{
  if(argc < 2){
    std::cerr << "input image file path." << std::endl;
    return 1;
  }
  cv::Mat img = cv::imread(argv[1], 1);
  if(img.empty()){
    std::cerr << "failed to open image file." << std::endl;
    return 2;
  }
  for(;;){
    cv::Point const center(img.cols / 2, img.rows / 2);
    double scale = 1.0;
    for(int deg = 0; deg < 360; ++deg){
      cv::Mat affine = cv::getRotationMatrix2D(center, deg, scale);
      cv::Mat rot;
      cv::warpAffine(img, rot, affine, img.size(), cv::INTER_CUBIC);
      cv::imshow("rotate", rot);
      cv::waitKey(10);
    }
  }
  return 0;
}

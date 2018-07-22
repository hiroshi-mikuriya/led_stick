#include "stick_sdk.h"
#include <opencv2/opencv.hpp>

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
    cv::Mat img = cv::imread(argv[1], 1);
    if(img.empty()){
        std::cerr << "failed to open image file." << std::endl;
        return 3;
    }
    cv::flip(img, img, -1);
    int const lines = 1364;
    cv::resize(img, img, cv::Size(lines, 32));
    img /= 64; // make colors ranging from 0 - 3.
    for(int x = 0; x < img.cols; ++x){
        char a[32 * 3] = { 0 };
        for(int y = 0; y < 32; ++y){
            cv::Vec3b const bgr = img.at<cv::Vec3b>(y, x);
            a[y * 3] = bgr[2];
            a[y * 3 + 1] = bgr[1];
            a[y * 3 + 2] = bgr[0];
        }
        write_line(x, a);
    }
    std::cerr << "completed." << std::endl;
    return 0;
}

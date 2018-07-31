#include <opencv2/opencv.hpp>
#include "stick_sdk.h"

namespace
{
    int const TEXT_WIDTH = 20;

    std::vector<cv::Scalar> const TEXT_COLORS {
        cv::Scalar(0xFF, 0x00, 0x00),
        cv::Scalar(0xFF, 0xFF, 0x00),
        cv::Scalar(0x00, 0xFF, 0x00),
        cv::Scalar(0x00, 0xFF, 0xFF),
        cv::Scalar(0x00, 0x00, 0xFF),
        cv::Scalar(0xFF, 0x00, 0xFF)
    };

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
        for(int i = 0;;i = (i + 1) % lines){
            for(int j = 0; j < 100; ++j){
                short a[3] = { 0 };
                get_accel(a);
                int line = (a[1] + 0x8000) * TEXT_WIDTH / 0x10000 + i;
                show_line(line);
            }
        }
    }

    cv::Mat text_image(std::string const text)
    {
        cv::Mat img = cv::Mat::zeros(32, text.size() * TEXT_WIDTH, CV_8UC3);
        for(size_t i = 0; i < text.size(); ++i){
            const char c[2] = { text[i] };
            auto const pt = cv::Point(i * TEXT_WIDTH, img.rows * 0.8);
            auto const color = TEXT_COLORS[i % TEXT_COLORS.size()];
            cv::putText(img, c, pt, cv::FONT_HERSHEY_SIMPLEX, 1, color, 2, CV_AA);
        }
        return img;
    }
}

int main(int argc, const char * argv[])
{
    if(argc < 2){
        std::cerr << "input some texts." << std::endl;
        return 1;
    }
    if(!init_sdk()){
        std::cerr << "failed to init stick sdk." << std::endl;
        return 2;
    }
    std::string const text(argv[1]);
    auto img = text_image(text);
    cv::flip(img, img, 1);
    int const lines = text.size() * TEXT_WIDTH;
    std::cerr << "writing image..." << std::endl;
    write(img, lines);
    std::cerr << "complete!" << std::endl;
    show(lines);
    return 0;
}

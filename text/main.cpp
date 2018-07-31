#include <opencv2/opencv.hpp>

namespace
{
    /*
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
            int line = (a[1] + 0x8000) / 48;
            if(0 <= line && line < lines){
                show_line(line);
            }
        }
    }
    */

    cv::Mat text_image(std::string const text)
    {
        int const w = 20;
        std::vector<cv::Scalar> const colors { 
            cv::Scalar(0xFF, 0x00, 0x00), cv::Scalar(0xFF, 0xFF, 0x00),
            cv::Scalar(0x00, 0xFF, 0x00), cv::Scalar(0x00, 0xFF, 0xFF),
            cv::Scalar(0x00, 0x00, 0xFF), cv::Scalar(0xFF, 0x00, 0xFF) };
        cv::Mat img = cv::Mat::zeros(32, text.size() * w, CV_8UC3);
        for(size_t i = 0; i < text.size(); ++i){
            const char a[2] = { text[i] };
            cv::putText(img, a, cv::Point(i * w, img.rows * 0.8), cv::FONT_HERSHEY_SIMPLEX, 1, colors[i % colors.size()], 2, CV_AA);
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
    /*
    if(!init_sdk()){
        std::cerr << "failed to init stick sdk." << std::endl;
        return 2;
    }
    */
    auto img = text_image(argv[1]);
    // cv::flip(img, img, 1);
    cv::imshow("text", img);
    cv::waitKey(0);
    /*
    int const lines = 1364;
    std::cerr << "writing image..." << std::endl;
    write(img, lines);
    std::cerr << "complete!" << std::endl;
    show(lines);
    */
    return 0;
}

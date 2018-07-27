#include "stick_sdk.h"
#include "mycamera.hpp"
#include <thread>

namespace
{
    bool s_button_event = false;

    int sampling_button(int count)
    {
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
        for(;!s_button_event;){
            short gyro[3] = { 0 };
            get_accel(gyro);
            int line = (gyro[1] + 0x8000) / 40;
            if(0 <= line && line < lines){
                show_line(line);
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    static_cast<void>(argc); // unused
    static_cast<void>(argv); // unused
    if(!init_sdk()){
        std::cerr << "failed to init stick sdk." << std::endl;
        return 1;
    }
    MyCamera cam(0);
    if(!cam.isOpened()){
        std::cerr << "failed to open camera device." << std::endl;
        return 2;
    }
    stop_led_demo();
    button_monitor();
    std::thread th([]{
        button_monitor();
    });
    for(;;){
        s_button_event = false;
        cv::Mat img;
        for(int i = 0; i < 10; ++i){
            cam >> img;
            sleep(10);
        }
        if(img.empty()){
            std::cerr << "failed to open image file." << std::endl;
            continue;
        }
        // cv::flip(img, img, 1);
        int const lines = 1364;
        std::cerr << "writing image..." << std::endl;
        write(img, lines);
        std::cerr << "complete!" << std::endl;
        show(lines);
    }
    // unreachable.
    th.join();
    return 0;
}

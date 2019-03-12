#include "stick.h"
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <opencv2/opencv.hpp>

namespace
{
	void write(cv::Mat m, int lines)
	{
		cv::Mat img;
		cv::resize(m, img, cv::Size(lines, COUNT_OF_LED));
		for (int x = 0; x < img.cols; ++x) {
			uint8_t a[COUNT_OF_LED * 3] = { 0 };
			for (int y = 0; y < img.rows; ++y) {
				cv::Vec3b const bgr = img.at<cv::Vec3b>(y, x);
				a[y * 3] = bgr[2] / 64; // 0 - 255 => 0 - 3
				a[y * 3 + 1] = bgr[1] / 64;
				a[y * 3 + 2] = bgr[0] / 64;
			}
			if (write_line(x, a) != 0)
				std::exit(1);
		}
	}

	void show(int lines)
	{
		for(;;){
			short a[3] = { 0 };
			if (get_accel(a) != 0)
				std::exit(1);
			int line = (a[1] + 0x8000) * lines / 0x10000;
			if (show_line(line) != 0)
				std::exit(1);
			usleep(1000);
		}
	}
}

int main(int argc, const char * argv[])
{
	if (argc < 2) {
		std::cerr << "input image file path." << std::endl;
		return 1;
	}
	if (stick_init() != 0)
		return 1;
	if (stop_led_demo() != 0)
		return 1;
	cv::Mat img = cv::imread(argv[1], 1);
	if (img.empty()) {
		std::cerr << "failed to open image file." << std::endl;
		return 3;
	}
	cv::flip(img, img, 1);
	int const lines = COUNT_OF_LINES;
	std::cerr << "writing image..." << std::endl;
	write(img, lines);
	std::cerr << "complete!" << std::endl;
	show(lines);
	return 0;
}

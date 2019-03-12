#include "stick.h"
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <opencv2/opencv.hpp>

namespace
{
	const int IMG_WIDTH = COUNT_OF_LINES / 10;

	void write(const char * file, int n)
	{
		cv::Mat img = cv::imread(file, 1);
		if (img.empty()) {
			std::cerr << "failed to open " << file << std::endl;
			std::exit(1);
		}
		cv::flip(img, img, 1);
		cv::resize(m, img, cv::Size(IMG_WIDTH, COUNT_OF_LED));
		int offset = n * IMG_WIDTH;
		for (int x = 0; x < img.cols; ++x) {
			uint8_t a[COUNT_OF_LED * 3] = { 0 };
			for (int y = 0; y < img.rows; ++y) {
				cv::Vec3b const bgr = img.at<cv::Vec3b>(y, x);
				a[y * 3] = bgr[2] / 64; // 0 - 255 => 0 - 3
				a[y * 3 + 1] = bgr[1] / 64;
				a[y * 3 + 2] = bgr[0] / 64;
			}
			if (write_line(x + offset, a) != 0)
				std::exit(1);
		}
	}

	void show(int count)
	{
		int n = 0; // TODO: ボタン押下でoffsetを更新する。(0 から count - 1 まで)
		for (;;) {
			int offset = n * IMG_WIDTH;
			short a[3] = { 0 };
			if (get_accel(a) != 0)
				std::exit(1);
			int line = (a[1] + 0x8000) * IMG_WIDTH / 0x10000;
			if (show_line(offset + line) != 0)
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
	std::cerr << "writing image..." << std::endl;
	for (int i = 0; i < argc - 1; ++i)
		write(argv[i + 1], i);
	std::cerr << "complete!" << std::endl;
	show(argc - 1);
	return 0;
}

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
		cv::resize(img, img, cv::Size(IMG_WIDTH, COUNT_OF_LED));
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

	void show(int nimgs)
	{
		for (int n = 0, btn0 = 0;;) {
			int btn1 = get_button();
			if (btn0 == 1 && btn1 == 0) // TODO: チャタリング対策必要？
				n = (n + 1) % nimgs;
			btn0 = btn1;
			int offset = n * IMG_WIDTH;
			short accel[3] = { 0 };
			if (get_accel(accel) != 0)
				std::exit(1);
			int line = (accel[1] + 0x8000) * IMG_WIDTH / 0x10000;
			if (show_line(offset + line) != 0)
				std::exit(1);
			usleep(1000);
		}
	}
}

int main(int argc, const char * argv[])
{
	const int nimgs = argc - 1;
	if (nimgs <= 0) {
		std::cerr << "input image file path." << std::endl;
		return 1;
	}
	if (stick_init() != 0 || stop_led_demo() != 0)
		return 1;
	std::cerr << "writing image..." << std::endl;
	for (int i = 0; i < nimgs; ++i)
		write(argv[i + 1], i);
	std::cerr << "complete!" << std::endl;
	show(nimgs);
	return 0;
}

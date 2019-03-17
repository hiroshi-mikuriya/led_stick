#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	const int gpio = 18;
	const int div = 1920;
	const uint8_t channel = 0;
	const uint8_t markspace = 1;
	const uint8_t enabled = 1;
	const uint32_t range = 200;
	if (bcm2835_init() != 1)
		return 1;
	bcm2835_gpio_fsel(gpio ,BCM2835_GPIO_FSEL_ALT5);
	bcm2835_pwm_set_clock(div);
	bcm2835_pwm_set_mode(channel, markspace, enabled);
	bcm2835_pwm_set_range(channel, range);
	for (;;) {
		int a = 0;
		scanf("%d", &a);
		bcm2835_pwm_set_data(channel, a);
	}
	return 0;
}

#include <stdio.h>
#include <unistd.h>

#include "../gpio.h"

int main() {
  if (gpio_init()) return 1;
  for (;;) {
    for (int pin = 16; pin < 32; ++pin) {
      gpio_configure(pin, GPIO_INPUT);
      // gpio_set_pull(pin, GPIO_PULLUP);
      if ((pin % 8) == 0) puts("");
      printf("PIN:%2d(%u)  ", pin, gpio_read(pin));
    }
    puts("");
    sleep(1);
  }
  if (gpio_deinit()) return 1;
  return 0;
}

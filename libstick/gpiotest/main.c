#include "../gpio.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    if (gpio_init() != 0)
        return 1;
    for (;;) {
        gpio_read(5);
        sleep(1);
    }
    if (gpio_deinit() != 0)
        return 1;
    return 0;
}

#include "../gpio.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    if (gpio_init() != 0)
        return 1;
    int pin = 5;
    for (;;) {
        printf("PIN:%d  LEV:%d\n", pin, gpio_read(pin));
        sleep(1);
    }
    if (gpio_deinit() != 0)
        return 1;
    return 0;
}

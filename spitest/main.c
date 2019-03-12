#include "spi.h"
#include <stdio.h>

int main()
{
    if (spi_init() != 0) {
        return 1;
    }
    uint8_t d[] = { 2, 0, 2, 0, 0 }; // stop demo
    if (spi_write(d, sizeof(d), 0) != 0) {
        return 1;
    }
    if (spi_deinit() != 0) {
        return 1;
    }
    return 0;
}
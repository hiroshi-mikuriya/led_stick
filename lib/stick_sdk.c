// gcc -shared -fPIC -I/usr/local/include -o stick_sdk.so stick_sdk.c

#include <bcm2835.h>
#include "stick_sdk.h"

static int g_sdk_is_initialized = 0;
static const int g_table[] = { 20, 18, 16, 10, 8, 22, 0, 14, 12, 6, 4, 2 };

static void write_spi(char * d, unsigned int len, unsigned char cs)
{
    if(g_sdk_is_initialized){
        bcm2835_spi_begin();
        bcm2835_spi_setBitOrder(1); // MSB First
        bcm2835_spi_setDataMode(0); // CPOL = 0, CPHA = 0
        bcm2835_spi_setClockDivider(128);
        bcm2835_spi_chipSelect(cs);
        bcm2835_spi_setChipSelectPolarity(cs, 0); // LOW;
        bcm2835_spi_writenb(d, len);
        bcm2835_spi_end();
    }
}

int init_sdk(void)
{
    return g_sdk_is_initialized = bcm2835_init();
}

void stop_led_demo(void)
{
    char d[] = { 2, 0, 2, 0, 0 };
    write_spi(d, sizeof(d), 0);
}

void write_led_pattern(int line, char * pattern)
{
    char d[24 + 3] = { 2 };
    int addr = 0x8000 + line * 24;
    d[1] = (addr >> 8) & 0xFF;
    d[2] = addr & 0xFF;
    for(int led = 0; led < 32; ++led){
        for(int rgb = 0; rgb < 3; ++rgb){
            char v = pattern[led * 3 + rgb] & 0x03;
            int n = g_table[(led % 4) * 3 + rgb] + (led / 4) * 24;
            int i0 = n / 8;
            int i1 = n % 8;
            d[i0 + 3] |= v << i1;
        }
    }
    write_spi(d, sizeof(d), 0);
}

void show_line(int line)
{
    char d[] = { 2, 0, 0, (line >> 8) & 0xFF, line & 0xFF };
    write_spi(d, sizeof(d), 0);
}

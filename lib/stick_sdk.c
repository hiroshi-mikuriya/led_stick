// gcc -shared -fPIC -I/usr/local/include  -o stick_sdk.so stick_sdk.c /usr/local/lib/libbcm2835.a

#include <bcm2835.h>
#include "stick_sdk.h"

#define REG_PWR_MGMT_1    0x6B
#define REG_INT_PIN_CFG   0x37
#define REG_GYRO_CONFIG   0x1B
#define REG_ACCEL_CONFIG1 0x1C
#define REG_ACCEL_CONFIG2 0x1D

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

static void write_i2c(unsigned char slave, unsigned char cmd, unsigned char d)
{
    char buf[] = { cmd, d };
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(slave);
    bcm2835_i2c_setClockDivider(2500);
    bcm2835_i2c_write(buf, sizeof(buf));
    bcm2835_i2c_end();
}

static void read_i2c(unsigned char slave, unsigned char cmd, char * buffer, int len)
{
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(slave);
    bcm2835_i2c_setClockDivider(2500);
    char c[] = { cmd };
    bcm2835_i2c_write(c, sizeof(c));
    bcm2835_i2c_read(buffer, len);
    bcm2835_i2c_end();
}

int init_sdk(void)
{
    if(g_sdk_is_initialized = bcm2835_init()){
        write_i2c(0x68, REG_PWR_MGMT_1, 0x80); // reset register
        write_i2c(0x68, REG_PWR_MGMT_1, 0x00); // clear power management
        write_i2c(0x68, REG_INT_PIN_CFG, 0x02); // enable AK8963 using I2C
        write_i2c(0x68, REG_ACCEL_CONFIG1, 0x08); // mod acceleration sensor range.
    }
    return g_sdk_is_initialized;
}

void stop_led_demo(void)
{
    char d[] = { 2, 0, 2, 0, 0 };
    write_spi(d, sizeof(d), 0);
}

void write_line(int line, char * pattern)
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

void get_gyro(short * gyro)
{
    char b[14] = { 0 };
    read_i2c(0x68, 0x3B, b, sizeof(b));
    for(int i = 0; i < 7; ++i){
        unsigned char u0 = b[i * 2] & 0xFF;
        unsigned char u1 = b[i * 2 + 1] & 0xFF;
        gyro[i] = (short)((u0 << 8) + u1);
    }
}
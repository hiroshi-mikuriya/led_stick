// gcc -shared -fPIC -I/usr/local/include  -o stick_sdk.so stick_sdk.c /usr/local/lib/libbcm2835.a

#include <bcm2835.h>
#include "stick_sdk.h"
#include "mpu9250reg.h"

#define GPIO_BUTTON  5

static int g_sdk_is_initialized = 0;
static const int g_table[] = { 20, 18, 16, 10, 8, 22, 0, 14, 12, 6, 4, 2 };

static void write_spi(char * d, unsigned int len, unsigned char cs)
{
  if(g_sdk_is_initialized){
    bcm2835_spi_chipSelect(cs);
    bcm2835_spi_setChipSelectPolarity(cs, 0); // LOW
    bcm2835_spi_writenb(d, len);
    bcm2835_spi_setChipSelectPolarity(cs, 1); // HI
  }
}

static void write_i2c(unsigned char slave, unsigned char cmd, unsigned char d)
{
  if(g_sdk_is_initialized){
    char buf[] = { cmd, d };
    bcm2835_i2c_setSlaveAddress(slave);
    bcm2835_i2c_write(buf, sizeof(buf));
  }
}

static void read_i2c(unsigned char slave, unsigned char cmd, char * buffer, int len)
{
  if(g_sdk_is_initialized){
    bcm2835_i2c_setSlaveAddress(slave);
    char c[] = { cmd };
    bcm2835_i2c_write(c, sizeof(c));
    bcm2835_i2c_read(buffer, len);
  }
}

int init_sdk(void)
{
  g_sdk_is_initialized = bcm2835_init();
  if(g_sdk_is_initialized){
    bcm2835_gpio_fsel(GPIO_BUTTON, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(GPIO_BUTTON, BCM2835_GPIO_PUD_UP);
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(slave);
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
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
  write_spi(d, sizeof(d), BCM2835_SPI_CS0);
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
  write_spi(d, sizeof(d), BCM2835_SPI_CS0);
}

void show_line(int line)
{
  char d[] = { 2, 0, 0, (char)((line >> 8) & 0xFF), (char)(line & 0xFF) };
  write_spi(d, sizeof(d), BCM2835_SPI_CS0);
}

static void get_3params(short * dst, unsigned char slave, unsigned char addr)
{
  char b[6] = { 0 };
  read_i2c(slave, addr, b, sizeof(b));
  for(int i = 0; i < 3; ++i){
    unsigned char u0 = b[i * 2] & 0xFF;
    unsigned char u1 = b[i * 2 + 1] & 0xFF;
    dst[i] = (short)((u0 << 8) + u1);
  }
}

void get_accel(short * accel)
{
  get_3params(accel, 0x68, REG_ACCEL_XOUT_H);
}

void get_gyro(short * gyro)
{
  get_3params(gyro, 0x68, REG_GYRO_XOUT_H);
}

int get_button(void)
{
  return bcm2835_gpio_lev(GPIO_BUTTON);
}

void sleep(int ms)
{
  bcm2835_delay(ms);
}

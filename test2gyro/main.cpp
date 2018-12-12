#include <bcm2835.h>
#include <iostream>
#include "mpu9250reg.h"

namespace
{
    void write_i2c(unsigned char slave, unsigned char cmd, unsigned char d)
    {
        char buf[] = { cmd, d };
        bcm2835_i2c_setSlaveAddress(slave);
        bcm2835_i2c_write(buf, sizeof(buf));
    }

    void read_i2c(unsigned char slave, unsigned char cmd, char * buffer, int len)
    {
        bcm2835_i2c_setSlaveAddress(slave);
        char c[] = { cmd };
        bcm2835_i2c_write(c, sizeof(c));
        bcm2835_i2c_read(buffer, len);
    }
    void get_3params(short * dst, unsigned char slave, unsigned char addr)
    {
        char b[6] = { 0 };
        read_i2c(slave, addr, b, sizeof(b));
        for(int i = 0; i < 3; ++i){
            unsigned char u0 = b[i * 2] & 0xFF;
            unsigned char u1 = b[i * 2 + 1] & 0xFF;
            dst[i] = (short)((u0 << 8) + u1);
        }
    }

    void get_accel(unsigned char slave, short * accel)
    {
        get_3params(accel, slave, REG_ACCEL_XOUT_H);
    }

    void get_gyro(unsigned char slave, short * accel)
    {
        get_3params(accel, slave, REG_GYRO_XOUT_H);
    }
}

int main()
{
    if(!bcm2835_init()){
        std::cerr << "failed to init bcm2835" << std::endl;
        return 1;
    }
    bcm2835_i2c_begin();
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
    for(;;){
        short a[6] = { 0 };
#if 1
        get_accel(0x68, a);
        get_accel(0x69, a + 3);
#else
        get_gyro(0x68, a);
        get_gyro(0x69, a + 3);
#endif
        for(int i = 0; i < 6; ++i){
            std::cout.width(8);
            std::cout << a[i];
        }
        std::cout << std::endl;
        asleep(100);
    }
    return 0;
}
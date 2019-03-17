#include "mpu6050.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	if (init_mpu6050(0x68))
		return 1;
	for (;;) {
		short accel[3] = { 0 };
		short gyro[3] = { 0 };
		if (read_accel(accel))
			return 1;
		if (read_gyro(gyro))
			return 1;
		printf("accel:%7d%7d%7d  gyro:%7d%7d%7d\n", accel[0], accel[1], accel[2],
			   gyro[0], gyro[1], gyro[2]);
		usleep(100 * 1000);
	}
	return 0;
}

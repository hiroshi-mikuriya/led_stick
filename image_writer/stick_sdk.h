#ifndef STICK_SDK__
#define STICK_SDK__

/**
 * @return 1: success 0: fail
 */
int init_sdk(void);

void stop_led_demo(void);

/**
 * @param[in] line 0 - 1364
 * @param[in] pattern LED flash pattern. It should has 32 * 3 items. RGB value should be between 0 and 3.
 */
void write_line(int line, char * pattern);

void show_line(int line);

/**
 * @param[out] gyro buffer size = 6
 */
void get_gyro(short * gyro);

#endif // STICK_SDK__
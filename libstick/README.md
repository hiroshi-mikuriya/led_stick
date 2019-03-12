libstick.so
======

# About

LEDスティックを制御するライブラリ

# Setup

```
$ cd libstick
$ make  # make libstick.so
$ sudo make install  # install libstick.so & stick.h
$ sudo ldconfig
```

# Test

SPI test

```
$ cd spitest
$ make
$ ./spitest
```

I2C test

```
$ cd i2ctest
$ make
$ ./i2ctest
```
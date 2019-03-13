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

SPIテスト（LED消灯・点灯を周期的に実施）

```
$ cd spitest
$ make
$ ./spitest
```

I2Cテスト（MPU6050のレジスタを定期的に読み出す）

```
$ cd i2ctest
$ make
$ ./i2ctest
```

GPIOテスト（ボタンのGPIOレベルを定期的に読み出す）

```
$ cd gpiotest
$ make
$ ./gpiotest
```
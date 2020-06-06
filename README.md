LEDスティック
======

|ディレクトリ|説明|
|:---|:---|
|doc|Yahoo Hack Dayで使ったポスターの原稿|
|images|スティックに表示する画像ファイル|
|libstick|スティックを制御するライブラリ|
|show_pics|libstickとOpenCVを使って画像ファイルをスティックに表示するプログラム|

## Setup

install libraries.

```
$ sudo apt-get install -y libopencv-dev cmake clang-format
```

enable I2C, SPI

```
$ sudo raspi-config
```

install application

```
$ sudo bash install.sh
```
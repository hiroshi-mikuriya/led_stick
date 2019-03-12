libstick.soを使うサンプル
=====

# About

libstick.soとOpenCVを使って、画像イメージをスティックに表示する

# Setup

```
$ make
$ ./show_pics ../images/android.jpg
```

# Auto Start

```
$ sudo ln -s /home/pi/led_stick/show_pics/show_pics.service /etc/systemd/system
$ sudo systemctl enable show_pics
$ sudo systemctl restart show_pics
```
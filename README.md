# LED STICK SDK

## ディレクトリ構成

* camera  
PIカメラで撮影しスティックに表示するプログラム  
ボタンを押すと撮影実行する。  
あらかじめPI Cameraのライブラリをインストールしておくこと。
* images  
picture files
* lib  
SDK source
* picamera  
PI Cameraのライブラリ  
使用する場合、install.shを実行する。
* python  
test mpu9250
* ruby  
SDK sample
* sanko
* show_pic  
画像ファイルをスティックに表示するプログラム

## Raspberry Pi開発環境構築実施手順

※配布したSDカードには実施済

[バックアップデータをダウンロード](https://drive.google.com/open?id=1TwVP-dZgIXtYlnaNzhRqqs8cp-seA_qB)


## Raspi SDカード

### バックアップ手順（Mac用）

SDカードをMacに挿す。  
ドライブ番号を調べる。  
/Volumes/boot が /dev/dusk4s1なので/dev/disk4がSDカードである。

```
$ df
Filesystem    512-blocks       Used Available Capacity iused               ifree %iused  Mounted on
/dev/disk4s1       86872      44839     42033    52%       0                   0  100%   /Volumes/boot
```

バックアップを実行する。  
１時間弱所要する。

```
$ sudo dd if=/dev/disk4 | gzip > backup`date +%Y%m%d`.img.gz
```

### 書き込み手順（Mac用）

Etcherというツールを使う。（無料）  
上記でバックアップしたファイルと、書き込み先のSDカードを指定して実行する。  
30分程度所要する。
# robosysled
## 概要
Raspberry PiでLEDを点灯させる
## インストール方法
```
$ https://github.com/takeda1350/robosysled.git
```
## 実行方法
```
$ cd robosysled
$ make
$ sudo insmod myled.ko
$ sudo chmod 666 /dev/myled0
```
デバイスファイルに'1'を書き込むとLEDが点滅する
```
$ echo 1 > /dev/myled0
```
## 後処理
以下のコマンドでデバイスファイルを削除する
```
$ sudo rmmod myled
```
##ライセンス
This repository is licensed under the GPLv3 license, see LICENSE

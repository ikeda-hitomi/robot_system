# robot_system

LEDを制御するデバイスドライバでモールス信号を表現する。  
今回はSUSHI(・・・　・・－　・・・　・・・・　・・)と表現した。

## 環境
- ハードウェア
Raspberry Pi 3 Model B+
- ソフトウェア
Ubuntu18.04

## 使用する道具
- ブレッドボード
- LED
- 抵抗(220Ω)
- ジャンパ線(オス~メス)

## 回路
GPIO 25(22番ピン)にLEDのアノードを繋ぎ、GND(39番ピン)にLEDのかカソードを繋ぐ。
## 実行方法
```
$ git clone https://github.com/ikeda-hitomi/robot_system.git
$ make
$ sudo insmod myled.ko
$ sudo chmod 666 /dev/myled0
```
カーネルモジュールをアンインストールする場合は、以下を実行する。
```
$ sudo rmmod myled
```
## LEDを点滅させる
```
$ echo 1 > /dev/myled0
$ echo 0 > /dev/myled0
```
LEDはecho 1の時に点滅し、echo 0の時に消灯する。   
動作動画:https://youtu.be/HFTvdFF50D8

## ライセンス
このリポジトリはGPLv3が付与されています。

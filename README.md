# robot_system

## 実行方法
```
$ make
$ sudo insmod myled
$ sudo chmod 666 /dev/myled0

```
## LEDを点滅させる
```
$ echo 1 > /dev/myled0
$ echo 0 > /dev/myled0
```

## ライセンス
このリポジトリはGPLv3が付与されています。

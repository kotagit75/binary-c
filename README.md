# binary-c
<img src="./img/icon.jpg" width= "200px" >

* バイナリを最適化します。

# How to?
* バイナリを多細胞生物に見立て、実際に動かし最適化をします。

# 環境設定
1. C言語ライブラリのインストール
* libjansson-dev(aptでインストール可)

2. 環境変数の設定
* .bash_profileに
```
export BINC="(パス)/binary-c/"
PATH=$PATH:$BINC./public
```
を追加。（これは、binary-cのパスを環境変数にし、そのpublicディレクトリにパスを通している）

# システム要件
* 環境:Linux
* c言語コンパイラ必要(gccで実行確認)
* ライブラリ:jansson（json読み込みのため）

# 参考文献
* バイナリ生成環境「daisy-tools」実験報告（著:⼤神祐真）:http://yuma.ohgami.jp/Daisy-Tools_Laboratory_Report/

# License
[MIT License](./LICENSE)
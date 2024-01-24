# 卒業制作「ローグライクゲーム」

## 実行方法
まずこのリポジトリをクローンするか、zipファイルをダウンロードして解凍してください。
```
（コンパイルから実行）
sudo apt-get install make -y
sudo apt-get install g++ -y
cd Rogue
make play

（AppImageから実行）
cd Rogue
chmod +x Rogue-x86_64.AppImage
./Rogue-x86_64.AppImage
```
AppImageで実行すると、たまに壁の中に敵が湧きます。

## ダンジョンの生成方法一覧
|生成方法  |備考|
|:-------:|:--|
|区域分割法|メジャーな方法です。全体をランダムに分割して、それぞれの領域に部屋を作ります。|
|RRA      |Random Room Assignmentだと長いので、格好つけて略してます。生成方法は名前のとおりです。|

## キャラクター一覧
プレイヤーは ```（現在のレベル） * 100```のEXPでレベルアップ  
（例）Lv.1→Lv.2　100EXP（総EXP100）
　　　Lv.2→Lv.3　200EXP（総EXP300）
|敵の種族         |アイコン                               |HP  |ATK |DEF |EXP|
|:-------------:|:-----------------------------------:|:--:|:--:|:--:|:--|
|born（プレイヤー）|![born](sample/born.png)             | 100|  20|  20|なし|
|deka           |![deka](sample/deka.png)             |  70|  10|  10| 10|
|guri           |![guri](sample/guri.png)             |  30|  25|   5| 17|
|jelyf          |![jelyf](sample/jelyf.png)           |  40|  20|   5|  7|
|yummy          |![yummy](sample/yummy.png)           | 100|   5|   5| 18|
|crystal        |![crystal](sample/crystal.png)       |  50|   1|  30| 10|
|grosspider     |![grosspider](sample/grosspider.png) |  40|   7|   7|  7|
|iron           |![iron](sample/iron.png)             |  50|   8|  10| 15|
|tatsu          |![tatsu](sample/tatsu.png)           |  30|  20|  15|  7|
|ball           |![ball](sample/ball.png)             |  20|  15|  20|  5|

## ダンジョンのサンプル画像
![参考画像](sample/sample.png)

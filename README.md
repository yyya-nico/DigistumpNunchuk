# DigistumpNunchuk

====

I2C通信を使用するヌンチャク型コントローラをマウス代わりに使用します

## Description

以下のような特徴があります

- 標準的なHIDクラスに準拠したデバイスとして使用できる（＝PCに接続して自動的に使用できる状態になる）

- 部品を入手しやすい
    - ヌンチャク型コントローラはリサイクルショップで購入しやすい
    - Digisparkも1個400円くらいで購入できる

## Requirement

* ヌンチャク型コントローラ
* Digispark

## Usage

1. コントローラとdigisparkを結線します（以下の図を参照）  
<img src="./Digispark&Nunchuck.png" />

1. Arduinoでdigisparkに[ソフトウェア](./src/DigistumpNunchuk/DigistumpNunchuk.ino)を書き込みします  

1. 以下のようにしてマウスのようにコントローラを使用できます  
　スティック：カーソル移動  
　Zボタン：右クリック  
　Cボタン：左クリック  

     ※起動時に現在のスティック位置を基準とするため、起動するまではしばらくスティックを放しておく必要がある。
   
## Licence

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


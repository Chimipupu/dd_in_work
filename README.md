# dd_in_work
社内＠6/2のF/W個人開発リポジトリ

## スケジュール（暫定）

- 6/2(月) ... 設計
- 6/3(火) ... 設計
- 6/4(水) ... 実装
- 6/5(木) ... テスト
- 6/6(金) ... 成果報告、保守日

## 概要

- ESP32でWiFiを通してデータを送受信できること

『H/W』

- ESP32-S3 開発基板 ... 2台（送信、受信で各１台ずつ）

『S/W』

- [Arduino IDE Ver2.3.6](https://github.com/arduino/arduino-ide/releases/tag/2.3.6)
- [arduino-esp32 Ver3.2.0](https://github.com/espressif/arduino-esp32/releases/tag/3.2.0)

## 成果物

成果物は下記のGithubリポジトリに日々の成果を更新すること
Github ... https://github.com/Chimipupu/dd_in_work

- F/W一式
  - 設計書
  - ソースコード
  - F/Wのバイナリ
  - テスト結果

## 仕様（暫定）

『共通』

- F/W ... WiFi経由で送受信データを送受信できること
- OS ... 期間が1周間だけのため、工数がないのでRTOSは搭載しない

『TX(送信側)』

- 1)　UDPでRXに指定したLEDの色データを送信する
- 2)　電文は「`led (RGBの色コード)`」で送る

『RX(受信側)』

- 1)　TXから「`led (RGBの色コード)`」を受信するまでポーリング
- 2)　TXから指定されたLEDの色データをLEDに反映
- 3)　TXに「`led proc ok`」または「`led proc ng`」をレスポンス

### 通信フォーマット

- `led (RGBの色コード)`
    - 正常レスポンス：`led proc ok`
    - 異常レスポンス：`led proc ng`
# 【初版】詳細設計書

## 改版履歴
- 初版（2025/6/3） ... シーケンス図、ビルド構成を追記
- 初版（2025/6/2） ... 初版のため作成

## 1.仕様

『共通』

- F/W ... WiFi経由でデータを送受信できること
- OS ... ~~Amazon FreeRTOSを搭載する~~ 1週間しか工数がないため、RTOSは未搭載

- マイコン(SoC)
  - ESP32-S3-WROOM-1-N16R8
    - CPU ... Tensilica Xtensa LX7
    - Clock ... 240MHz
    - RAM ... 520KB
    - PSRAM ... 8MB
    - Flash ... 16MB
    - WiFi ... Wi-Fi(802.11b/g/n)@2.4GHz

```mermaid
sequenceDiagram
    TX(送信側) ->>+RX(受信側): WiFi通信リクエスト
    RX(受信側)-->>-TX(送信側): WiFi通信準備完了レスポンス
    TX(送信側) ->>+RX(受信側): LEDの色変更リクエスト
    RX(受信側)-->>-TX(送信側): 処理レスポンス(※正常系のみ)
```

『TX(送信側)』

- 1)　RXに指定したLEDの色データを送信する
- 2)　電文は「`led (RGBの色コード)`」で送る

『RX(受信側)』

- 1)　TXから「`led (RGBの色コード)`」を受信するまでポーリング
- 2)　TXから指定されたLEDの色データをLEDに反映
- 3)　TXに「`led proc ok`」または「`led proc ng`」をレスポンス

### 1.1.無線関連

- WiFi (IEEE802.11b/g/n)
- 送受信 ... UDPやESPNOWを使用すること

### 1.2.通信フォーマット

- `led (RGBの色コード)`
  - 正常レスポンス：`led proc ok`
  - 異常レスポンス：`led proc ng`

## 2.ビルド構成

- Arduino IDEでのビルド構成は下記図の通りである。

<div align="center">
  <img width="500" src="/doc/dd_in_work_arduinoide_build_info_20250603.png">
</div>

- ボード ... "ESP32S3 Dev Module"
- USB CDC On Boot ... "Disabled"
- CPU Frequency ... "240MHz (WiFi)"
- Core Debug Level ... "None"
- USB DFU On Boot ... "Disabled"
- Erase All Flash Before Sketch Upload ... "Disabled"
- Events Run On ... "Core 1"
- Flash Mode ... "QIO 120MHz"
- Flash Size ... "16MB (128Mb)"
- JTAG Adapter ... "Integrated USB JTAG"
- Arduino Runs On ... "Core 1"
- USB Firmware MSC On Boot ... "Disabled"
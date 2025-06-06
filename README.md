# ESP32の基礎研究 @ESPNOW

- 社内（＠6/2～6/6）で個人開発リポジトリ
  - ESPNOWの基礎研究をしていた

---

## 概要

<div align="center">
  <img width="500" src="/doc/dd_in_work_20250602.png">
</div>

『F/Wの実装目標』
- ESP32でWiFiを通してデータを送受信できること

『H/W』
- 私物のESP32-S3 開発基板（型番:[VCC-GND YD-ESP32-S3🔗](https://github.com/vcc-gnd/YD-ESP32-S3)）
  - TX(送信側)... WiFi MACアドレス(34:85:18:8E:ED:AC)
  - RX(受信側)... WiFi MACアドレス(34:85:18:46:B9:D8)

『開発環境IDE』

- [Arduino IDE ... Ver2.3.6](https://github.com/arduino/arduino-ide/releases/tag/2.3.6)
- [arduino-esp32 ... Ver3.2.0](https://github.com/espressif/arduino-esp32/releases/tag/3.2.0)

---
## 成果物

凡例(✅️...完了、📍...作業中、❌️...TBD)

- F/W一式
  - ✅️[設計書🔗](/doc/pj_design.md) ... [初版](/doc/pj_design.md)🔗
  - ✅️[ソースコード](/src/dd_in_work_esp32s3)
  - ✅️テストログ
    - 最新ログ（2025/6/5 REV29）
      - [TX(送信側)ログ](/doc/log/testlog_espnow_tx_20250605.log)
      - [RX(受信側)ログ](/doc/log/testlog_espnow_rx_20250605.log)

---
## スケジュール（暫定）

- 6/2(月) ... 設計
- 6/3(火) ... 設計
- 6/4(水) ... 実装
- 6/5(木) ... テスト
- 6/6(金) ... 保守
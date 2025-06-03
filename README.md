# dd_in_work
社内＠6/2のF/W個人開発リポジトリ

---
## 概要

<div align="center">
  <img width="500" src="/doc/dd_in_work_20250602.png">
</div>

『F/Wの実装目標』
- ESP32でWiFiを通してデータを送受信できること

『H/W』
- ESP32-S3 開発基板（私物）
  - TX(送信側)... WiFi MACアドレス(34:85:18:8E:ED:AC)
  - RX(受信側)... WiFi MACアドレス(34:85:18:46:B9:D8)

『開発環境IDE』

- [Arduino IDE ... Ver2.3.6](https://github.com/arduino/arduino-ide/releases/tag/2.3.6)
- [arduino-esp32 ... Ver3.2.0](https://github.com/espressif/arduino-esp32/releases/tag/3.2.0)

---
## 成果物

凡例(✅️...完了、📍...作業中、❌️...TBD)

- F/W一式
  - 📍[設計書🔗](/doc/pj_design.md) ... [/doc/pj_design.md](/doc/pj_design.md)🔗
  - 📍ソースコード .../src/dd_in_work_esp32s3
  - ❌️(TBD)F/Wのバイナリ ... /bin/*.uf2
  - ❌️(TBD)テスト結果 ... /doc/pj_test.md

---
## スケジュール（暫定）

- 6/2(月) ... 設計
- 6/3(火) ... 設計
- 6/4(水) ... 実装
- 6/5(木) ... テスト
- 6/6(金) ... 保守
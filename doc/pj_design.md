# 詳細設計書

## 改版履歴
- 2025/6/2 ... 初版のため作成

## 仕様（暫定）

『共通』

- F/W ... WiFi経由で送受信データを送受信できること
- OS ... Amazon FreeRTOSを搭載する

『TX(送信側)』

- 1)　UDPでRXに指定したLEDの色データを送信する
- 2)　電文は「`led (RGBの色コード)`」で送る

『RX(受信側)』

- 1)　TXから「`led (RGBの色コード)`」を受信するまでポーリング
- 2)　TXから指定されたLEDの色データをLEDに反映
- 3)　TXに「`led proc ok`」または「`led proc ng`」をレスポンス

### 無線関連

- WiFi (IEEE802.11b/g/n)
- 送受信 ... UDPやESPNOWを使用すること

### 通信フォーマット

- `led (RGBの色コード)`
  - 正常レスポンス：`led proc ok`
  - 異常レスポンス：`led proc ng`
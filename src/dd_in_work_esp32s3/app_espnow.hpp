#ifndef APP_ESPNOW_HPP
#define APP_ESPNOW_HPP

#include "common.hpp"
#include "esp_mac.h"
#include <esp_now.h>
#include <WiFi.h>

// ESPNOW送信、受信側のコンパイルスイッチ
// #define DD_ESP_TX
#define DD_ESP_RX

// 通信フォーマット
const char CMD_COM_REQ[] = "COM REQ";             // 通信要求リクエスト
const char RES_COM_OK[]  = "COM RES OK";          // 通信要求レスポンス

const char CMD_LED_REQ[] = "LED REQ ";            // LED色変更リクエスト
const char CMD_LED_REQ_RED[] = "LED REQ RED";     // LED色変更リクエスト 赤
const char CMD_LED_REQ_GREEN[] = "LED REQ GREEN"; // LED色変更リクエスト 緑
const char CMD_LED_REQ_BLUE[] = "LED REQ BLUE";   // LED色変更リクエスト 青
const char CMD_LED_REQ_WHITE[] = "LED REQ WHITE"; // LED色変更リクエスト 白
const char RES_LED_OK[]  = "LED RES OK";          // LED色変更レスポンス

void app_esp_init(void);
void app_esp_main(void);

#endif // APP_ESPNOW_HPP
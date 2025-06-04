#ifndef APP_ESPNOW_HPP
#define APP_ESPNOW_HPP

#include "common.hpp"
#include "esp_mac.h"
#include <esp_now.h>
#include <WiFi.h>

// ESPNOW送信、受信側のコンパイルスイッチ
// #define DD_ESP_TX
#define DD_ESP_RX

void app_esp_init(void);
void app_esp_main(void);

#endif // APP_ESPNOW_HPP
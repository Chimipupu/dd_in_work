#ifndef APP_ESPNOW_HPP
#define APP_ESPNOW_HPP

#include <stdint.h>
#include <string.h>
#include "esp_mac.h"
#include <esp_now.h>
#include <WiFi.h>

void app_esp_init(void);
void app_esp_main(void);

#endif // APP_ESPNOW_HPP
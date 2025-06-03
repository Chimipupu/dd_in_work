/**
 * @file dd_in_work_esp32s3.ino
 * @author dd yf
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdint.h>
#include <string.h>
#include "esp_mac.h"

static void mac_addr_print(void);
static String get_mac_addr(esp_mac_type_t mac_type);

/**
 * @brief MACアドレスを取得する関数
 * 
  * @param mac_type 
 * @return String MACアドレス文字列
 */
static String get_mac_addr(esp_mac_type_t mac_type)
{
    String mac = "";

    unsigned char mac_buf[6] = {0};
    memset(0x00, mac_buf, sizeof(mac_buf));

    if (esp_read_mac(mac_buf, mac_type) == ESP_OK) {
        char buf[18] = {0};
        memset(buf, 0x00, sizeof(buf));
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac_buf[0], mac_buf[1], mac_buf[2], mac_buf[3], mac_buf[4], mac_buf[5]);
        mac = buf;
    }

    return mac;
}

/**
 * @brief MACアドレス情報表示関数
 * 
 */
static void mac_addr_print(void)
{
    Serial.println("ESP32-S3 MAC Address Information's");

    Serial.print("WiFi STA MAC\t\t\t");
    Serial.println(get_mac_addr(ESP_MAC_WIFI_STA));

    Serial.print("WiFi AP MAC\t\t\t");
    Serial.println(get_mac_addr(ESP_MAC_WIFI_SOFTAP));

    Serial.print("Bluetooth MAC\t\t\t");
    Serial.println(get_mac_addr(ESP_MAC_BT));

    Serial.print("Ethernet MAC\t\t\t");
    Serial.println(get_mac_addr(ESP_MAC_ETH));
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
}

void loop()
{
    get_mac_addr();
    delay(5000);
}
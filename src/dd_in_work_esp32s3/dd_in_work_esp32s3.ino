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
#include <esp_now.h>
#include <WiFi.h>

#define DEBUG_DD_ESP // デバッグ用マクロ
#ifdef DEBUG_DD_ESP
static void mac_addr_print(void);
static String get_mac_addr(esp_mac_type_t mac_type);
#endif // DEBUG_DD_ESP

static void espnow_init(void);

#define DD_ESP_TX
#ifdef DD_ESP_TX
/**
 * @brief ESP-NOW TXメイン関数
 * 
 */
static void dd_tx_esp_main(void)
{
    // TODO
}
#endif // DD_ESP_TX

// #define DD_ESP_RX
#ifdef DD_ESP_RX
/**
 * @brief ESP-NOW rXメイン関数
 * 
 */
static void dd_rx_esp_main(void)
{
    // TODO
}
#endif // DD_ESP_RX


/**
 * @brief ESP-NOWの初期化関数
 * 
 */
static void espnow_init(void)
{
    esp_err_t ret = ESP_OK;

    WiFi.mode(WIFI_STA);
    ret = esp_now_init();

    if( ret != ESP_OK){
        Serial.println("[ERR] : ESP-NOW initialization failed");
    }else{
        Serial.println("[INF] : ESP-NOW initialization succeeded");
    }
}

#ifdef DEBUG_DD_ESP
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
    memset(mac_buf, 0x00, sizeof(mac_buf));

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
#endif

void setup()
{
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

#if defined(DD_ESP_TX)
    Serial.println("ESP_TX");
#elif defined(DD_ESP_RX)
    Serial.println("ESP_RX");
#endif

    espnow_init();

#ifdef DEBUG_DD_ESP
    mac_addr_print();
    delay(5000);
#endif
}

void loop()
{
#if defined(DD_ESP_TX)
    dd_tx_esp_main();
#elif defined(DD_ESP_RX)
    dd_rx_esp_main();
#endif
}
/**
 * @file dd_in_work_esp32s3.ino
 * @author dd yf
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "esp_mac.h"

static void get_mac_addr(void);

/**
 * @brief MACアドレスを取得する関数
 * 
 * @return String MACアドレス文字列
 */
String getDefaultMacAddress()
{
    String mac = "";

    unsigned char mac_base[6] = {0};

    if (esp_efuse_mac_get_default(mac_base) == ESP_OK) {
        char buffer[18];
        sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
        mac = buffer;
    }

    return mac;
    }

    String getInterfaceMacAddress(esp_mac_type_t interface) {

    String mac = "";

    unsigned char mac_base[6] = {0};

    if (esp_read_mac(mac_base, interface) == ESP_OK) {
        char buffer[18];
        sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
        mac = buffer;
    }

    return mac;
}

static void get_mac_addr(void)
{
    Serial.println("ESP32-S3 MAC Address Information's");

    Serial.print("Wi-Fi MAC\t\t\t");
    Serial.println(getDefaultMacAddress());

    Serial.print("WiFi STA MAC\t\t\t");
    Serial.println(getInterfaceMacAddress(ESP_MAC_WIFI_STA));

    Serial.print("WiFi AP MAC\t\t\t");
    Serial.println(getInterfaceMacAddress(ESP_MAC_WIFI_SOFTAP));

    Serial.print("Bluetooth MAC\t\t\t");
    Serial.println(getInterfaceMacAddress(ESP_MAC_BT));

    Serial.print("Ethernet MAC\t\t\t");
    Serial.println(getInterfaceMacAddress(ESP_MAC_ETH));
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
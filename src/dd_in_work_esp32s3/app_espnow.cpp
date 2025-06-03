#include "app_espnow.hpp"

#define DD_ESP_TX
// #define DD_ESP_RX

// TX(送信側)... WiFi MACアドレス(34:85:18:8E:ED:AC)
// RX(受信側)... WiFi MACアドレス(34:85:18:46:B9:D8)
uint8_t tx_mac_addr[] = {0x34, 0x85, 0x18, 0x8E, 0xED, 0xAC};
uint8_t rx_mac_addr[] = {0x34, 0x85, 0x18, 0x46, 0xB9, 0xD8};

struct ControlData{
    uint8_t mode;
    uint8_t brightness;
};

esp_now_peer_info_t peerInfo;

static void espnow_send_data(void);
static void cb_send_res(const uint8_t *mac_addr, esp_now_send_status_t status);

#define DEBUG_DD_ESP // デバッグ用マクロ
#ifdef DEBUG_DD_ESP
static void mac_addr_print(void);
static String get_mac_addr(esp_mac_type_t mac_type);
#endif // DEBUG_DD_ESP

static void espnow_init(void);

/**
 * @brief 送信完了コールバック関数
 * 
 * @param mac_addr 送信先のMACアドレス
 * @param status 送信ステータス
 */
static void cb_send_res(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if(status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("[INFO] : ESP-NOW data send succeeded");
    } else {
        Serial.println("[ERR] : ESP-NOW data send failed");
    }
}

/**
 * @brief ESP-NOWでのデータ送信関数
 * 
 */
static void espnow_send_data(void)
{
    ControlData data = {
        .mode = 1,
        .brightness = 5
    };

#ifdef DD_ESP_TX
    esp_now_send(rx_mac_addr, (uint8_t*)&data, sizeof(data));
#else
    esp_now_send(rx_mac_addr, (uint8_t*)&data, sizeof(data));
#endif
}

#ifdef DD_ESP_TX
/**
 * @brief ESP-NOW TXメイン関数
 * 
 */
static void dd_tx_esp_main(void)
{
    // TODO
    espnow_send_data();
}
#endif // DD_ESP_TX

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
        Serial.println("[INFO] : ESP-NOW initialization succeeded");
    }
}

void app_esp_init(void)
{
    espnow_init();

#ifdef DD_ESP_TX
    memcpy(peerInfo.peer_addr, rx_mac_addr, 6);
#else
    memcpy(peerInfo.peer_addr, tx_mac_addr, 6);
#endif

    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("[ERRs] : ESP-NOW peer mac addr add failed");
    }else{
        Serial.println("[INFO] : ESP-NOW peer mac addr add succeeded");
    }

    esp_now_register_send_cb(cb_send_res);
}

void app_esp_main(void)
{
#ifdef DD_ESP_TX
    dd_tx_esp_main();
#else
    dd_rx_esp_main();
#endif // DD_ESP_RX

    delay(2000);
}

// ↓デバッグ用
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
#endif // DEBUG_DD_ESP

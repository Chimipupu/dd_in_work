#include "app_espnow.hpp"
#include "app_led.hpp"

// TX(送信側)... WiFi MACアドレス(34:85:18:8E:ED:AC)
// RX(受信側)... WiFi MACアドレス(34:85:18:46:B9:D8)
uint8_t g_tx_mac_addr[] = {0x34, 0x85, 0x18, 0x8E, 0xED, 0xAC};
uint8_t g_rx_mac_addr[] = {0x34, 0x85, 0x18, 0x46, 0xB9, 0xD8};

// 送信データバッファ
uint8_t g_tx_data_buf[250] = {0};

bool g_com_req_flg = false;     // 通信要求フラグ
bool g_led_req_flg = false;     // LED色変更要求フラグ

bool g_rx_data_flg = false;     // 受信データフラグ
char g_rx_data_buf[250] = {0};  // 受信データバッファ
String g_req_color_str = "";    // LED色変更要求のRGB値の文字列

bool g_com_res_ok_flg = false;  // 通信要求レスポンスOKフラグ
bool g_led_res_ok_flg = false;  // LED色変更レスポンスOKフラグ

esp_now_peer_info_t g_espnow_peer_Info; // ESPNOWのピア（通信相手）情報

rgb_led_t g_rgb; // RGB LEDの色情報

static esp_err_t espnow_send_data(uint8_t *p_mac_addr, uint8_t *p_date_buf, uint8_t data_len);
static void cb_espnow_tx_data(const uint8_t *p_mac_addr, esp_now_send_status_t status);
static void cb_espnow_rx_data(const esp_now_recv_info_t *p_esp_now_recv_info, const uint8_t *p_rx_data, int data_len);
static void espnow_rx_data_parse(const uint8_t *p_rx_data);

#ifdef DEBUG_DD_ESP
static void mac_addr_print(void);
static String get_mac_addr(esp_mac_type_t mac_type);
bool g_debug_com_flg = true; // デバッグ用の通信フラグ
#endif // DEBUG_DD_ESP

/**
 * @brief ESPNOW送信完了時のコールバック関数(引数の順番、型はESP-NOWの仕様準拠)
 * 
 * @param mac_addr 送信先のMACアドレス
 * @param status 送信ステータス
 */
static void cb_espnow_tx_data(const uint8_t *p_mac_addr, esp_now_send_status_t status)
{
    if(status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("[INFO] : ESP-NOW data TX OK");
    } else {
        Serial.printf("[ERR] : ESP-NOW data TX NG. ERROR CODE : 0x%02X\r\n", status);
    }
}

static void cb_espnow_rx_data(const esp_now_recv_info_t *p_esp_now_recv_info, const uint8_t *p_rx_data, int data_len)
{
    g_rx_data_flg = true;
    memset(&g_rx_data_buf[0], 0x00, sizeof(g_rx_data_buf));

    Serial.println("[INFO] : ESP-NOW data RX!");
    Serial.printf("[INFO] : ESP-NOW Src MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
                    g_espnow_peer_Info.peer_addr[0], g_espnow_peer_Info.peer_addr[1],
                    g_espnow_peer_Info.peer_addr[2], g_espnow_peer_Info.peer_addr[3],
                    g_espnow_peer_Info.peer_addr[4], g_espnow_peer_Info.peer_addr[5]);

    char buf[250] = {0};
    memset(buf, 0x00, sizeof(buf));
    uint8_t copy_len =  (data_len < (sizeof(buf) - 1)) ? data_len : (sizeof(buf) - 1);
    memcpy(buf, p_rx_data, copy_len);
    buf[copy_len] = '\0';

    // String型にして末尾の空白や改行を削除
    String rx_str = String(buf);
    rx_str.trim();
    strncpy(g_rx_data_buf, rx_str.c_str(), sizeof(g_rx_data_buf) - 1);
    g_rx_data_buf[sizeof(g_rx_data_buf) - 1] = '\0';

    Serial.printf("[INFO] : ESP-NOW RX Data(size:%dByte) : %s\r\n", copy_len, rx_str.c_str());

    // 受信データのパース
    espnow_rx_data_parse((const uint8_t *)g_rx_data_buf);
}

/**
 * @brief ESP-NOWデータ送信関数
 * 
 * @param p_mac_addr 送信先のMACアドレスポインタ
 * @param p_date_buf 送信データバッファポインタ
 * @param data_len 送信データ長
 * @return esp_err_t 正常:ESP_OK ,異常:ESP_ERR_XXX
 */
static esp_err_t espnow_send_data(uint8_t *p_mac_addr, uint8_t *p_date_buf, uint8_t data_len)
{
    esp_err_t ret = ESP_OK;

    ret = esp_now_send(p_mac_addr, p_date_buf, data_len);
    Serial.printf("[INFO] : ESP-NOW Dst MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
                    p_mac_addr[0], p_mac_addr[1],
                    p_mac_addr[2], p_mac_addr[3],
                    p_mac_addr[4], p_mac_addr[5]);
    Serial.printf("[INFO] : ESP-NOW Send Data(size:%dByte) : %s\r\n", data_len, p_date_buf);

    return ret;
}

/**
 * @brief ESPNOW受信データのパース（解析）
 * 
 * @param p_rx_data 受信データポインタ
 */
static void espnow_rx_data_parse(const uint8_t *p_rx_data)
{
    char buf[250] = {0};
    memset(buf, 0x00, sizeof(buf));
    strncpy(buf, (const char *)p_rx_data, sizeof(buf) - 1);
    String rx_str = String(buf);
    rx_str.trim();

    if (rx_str.startsWith("COM REQ")) {
        Serial.println("[DEBUG] : COM REQ received");
        g_com_req_flg = true;
    } else if (rx_str.startsWith("COM RES OK")) {
        Serial.println("[DEBUG] : COM RES OK received");
        g_com_res_ok_flg = true;
    } else if (rx_str.startsWith("LED REQ")) {
        Serial.println("[DEBUG] : LED REQ received");
        g_led_req_flg = true;
        // "LED REQ"のRGB値を抽出
        int space_index = rx_str.indexOf(' ');
        if (space_index >= 0) {
            int second_space_index = rx_str.indexOf(' ', space_index + 1);
            if (second_space_index >= 0 && rx_str.length() > second_space_index + 1) {
                g_req_color_str = rx_str.substring(second_space_index + 1);
                g_req_color_str.trim();
                app_led_set_color(g_req_color_str);
                Serial.printf("[DEBUG] : LED REQ Color = %s\n", g_req_color_str.c_str());
            } else {
                Serial.println("[ERR] : LED REQ Color NG! illigal format");
                g_req_color_str = "";
                g_led_req_flg = false;
            }
        }
    } else if (rx_str.startsWith("LED RES OK")) {
        Serial.println("[DEBUG] : LED RES OK received");
#ifdef DD_ESP_TX
        char color_code[8] = {0}; // "#RRGGBB" + '\0' = 8文字
        Serial.printf("[DEBUG] : TX LED Color 0x%s\r\n", color_code);
        memset(color_code, 0x00, sizeof(color_code));
        snprintf(color_code, sizeof(color_code),"#%02X%02X%02X",
                g_rgb.r, g_rgb.g, g_rgb.b);
        app_led_set_color(color_code);
#endif // DD_ESP_TX
        g_led_res_ok_flg = true;
    } else {
        // NOP
    }

    g_rx_data_flg = false;
}

#ifdef DD_ESP_TX
/**
 * @brief ESP-NOW TXメイン関数
 * 
 */
static void dd_tx_esp_main(void)
{
    uint8_t data_len = 0;
    uint8_t *p_buf;

    // 通信要求リクエストの送信
    if (g_com_res_ok_flg != true) {
        data_len = strlen(&CMD_COM_REQ[0]);
        espnow_send_data(g_espnow_peer_Info.peer_addr, (uint8_t *)CMD_COM_REQ, data_len);
    }

    // LED色変更リクエストの送信
    if (g_com_res_ok_flg != false) {
        p_buf = &g_tx_data_buf[0];
        memset(&g_tx_data_buf[0], 0x00, sizeof(g_tx_data_buf));
        memcpy(g_tx_data_buf, CMD_LED_REQ, strlen(CMD_LED_REQ));
        p_buf += strlen(CMD_LED_REQ);
        app_led_req_data_gen(p_buf, &g_rgb);
        data_len = strlen((const char *)g_tx_data_buf);
        espnow_send_data(g_espnow_peer_Info.peer_addr, (uint8_t *)g_tx_data_buf, data_len);
        g_led_res_ok_flg = false;
    }
}
#else
/**
 * @brief ESP-NOW RXメイン関数
 * 
 */
static void dd_rx_esp_main(void)
{
    // 通信要求リクエストのレスポンスを送信
    if (g_com_req_flg != false) {
        espnow_send_data(g_espnow_peer_Info.peer_addr, (uint8_t *)RES_COM_OK, strlen(RES_COM_OK));
        g_com_req_flg = false;
        Serial.println("[DEBUG] : COM RES OK send");
    }

    // LED色変更リクエストのレスポンスを送信
    if (g_led_req_flg != false){
        espnow_send_data(g_espnow_peer_Info.peer_addr, (uint8_t *)RES_LED_OK, strlen(RES_LED_OK));
        g_led_req_flg = false;
        Serial.println("[DEBUG] : LED RES OK send");
    }
}
#endif // DD_ESP_TX

void app_esp_init(void)
{
    esp_err_t ret = ESP_OK;

    // WiFi初期化
    WiFi.mode(WIFI_STA);
    // WiFiを長距離通信モードに変更
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
    ret = esp_now_init();

    if( ret != ESP_OK){
        Serial.println("[ERR] : ESP-NOW initialization failed");
    }else{
        Serial.println("[INFO] : ESP-NOW initialization succeeded");
    }

    // 通信相手のMACアドレス設定
#ifdef DD_ESP_TX
    memcpy(g_espnow_peer_Info.peer_addr, g_rx_mac_addr, 6);
#else
    memcpy(g_espnow_peer_Info.peer_addr, g_tx_mac_addr, 6);
#endif

    g_espnow_peer_Info.channel = 0;
    g_espnow_peer_Info.encrypt = false;

    if (esp_now_add_peer(&g_espnow_peer_Info) != ESP_OK){
        Serial.println("[ERRs] : ESP-NOW peer mac addr add failed");
    }else{
        Serial.println("[INFO] : ESP-NOW peer mac addr add succeeded");
    }

    // ESPNOW送受信時のコールバック関数登録
    esp_now_register_send_cb(cb_espnow_tx_data);
    esp_now_register_recv_cb(cb_espnow_rx_data);
}

void app_esp_main(void)
{
#ifdef DD_ESP_TX
    dd_tx_esp_main();
#else
    dd_rx_esp_main();
#endif // DD_ESP_TX
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
#include "app_led.hpp"
#include "app_espnow.hpp"

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static void led_test(void);

rgb_led_t g_rgb;
static void rgb_val_inc(void);

/**
 * @brief LEDアプリ初期化
 * 
 */
void app_led_init(void)
{
    String str = "0,0,0";

    // LEDを消灯
    app_led_set_color(str);
}

/**
 * @brief LEDアプリメイン
 * 
 */
void app_led_main(void)
{
    // TODO
}

/**
 * @brief RGB値のインクリメント
 * 
 */
static void rgb_val_inc(void)
{
    if (g_rgb.r < 255) {
        g_rgb.r++;
    } else if (g_rgb.g < 255) {
        g_rgb.g++;
    } else if (g_rgb.b < 255) {
        g_rgb.b++;
    } else {
        g_rgb.r = 0;
        g_rgb.g = 0;
        g_rgb.b = 0;
    }
}

/**
 * @brief LED色変更リクエストのデータ生成関数
 * 
 */
void app_led_req_data_gen(uint8_t *p_tx_buf)
{
    char color_code[8] = {0}; // "#RRGGBB" + '\0' = 8文字

    // RGB値をインクリメント
    rgb_val_inc();

    // RGB値を "#RRGGBB" 形式に変換
    snprintf(color_code, sizeof(color_code), "#%02X%02X%02X", g_rgb.r, g_rgb.g, g_rgb.b);
    memcpy(p_tx_buf, CMD_LED_REQ, strlen(CMD_LED_REQ));
    size_t offset = strlen(CMD_LED_REQ);
    p_tx_buf += offset;
    memcpy(p_tx_buf, color_code, strlen(color_code));

#ifdef DD_ESP_TX
    Serial.printf("[DEBUG] : TX, Sync LED Color 0x%s\r\n", color_code);
    app_led_set_color(String(color_code));
#endif
}

/**
 * @brief ASCIIで指定された色名、RGBt値をLEDに設定する関数
 * 
 * @param color_name 
 */
void app_led_set_color(String color_name)
{
    color_name.trim();        // 空白や改行の除去
    color_name.toLowerCase(); // 大文字小文字の区別なし

    uint32_t color = pixels.Color(0, 0, 0);      // デフォルトは消灯

    if (color_name == "red") {              // 赤
        color = pixels.Color(255, 0, 0);
    } else if (color_name == "green") {     // 緑
        color = pixels.Color(0, 255, 0);
    } else if (color_name == "blue") {      // 青
        color = pixels.Color(0, 0, 255);
    } else if (color_name == "white") {     // 白
        color = pixels.Color(255, 255, 255);
    } else if (color_name.indexOf(',') != -1) {
        // RGB形式の色指定のパース（例 ... 0,128,255）
        int r, g, b;
        int firstComma = color_name.indexOf(',');
        int secondComma = color_name.indexOf(',', firstComma + 1);

        if (firstComma > 0 && secondComma > firstComma) {
            String r_str = color_name.substring(0, firstComma);
            String g_str = color_name.substring(firstComma + 1, secondComma);
            String b_str = color_name.substring(secondComma + 1);

            // 範囲チェック（0～255）
            r_str.trim();
            g_str.trim();
            b_str.trim();

            int r = constrain(r_str.toInt(), 0, 255);
            int g = constrain(g_str.toInt(), 0, 255);
            int b = constrain(b_str.toInt(), 0, 255);

            color = pixels.Color(r, g, b);
        }
    // RGBの16進数形式 "#RRGGBB"、"RRGGBB" に対応
    } else if ((color_name.startsWith("#") && color_name.length() == 7) || color_name.length() == 6) {
        String hex = color_name;
        if (hex.startsWith("#")) {
            hex = hex.substring(1);
        }

        char r_str[3] = { hex.charAt(0), hex.charAt(1), '\0' };
        char g_str[3] = { hex.charAt(2), hex.charAt(3), '\0' };
        char b_str[3] = { hex.charAt(4), hex.charAt(5), '\0' };

        int r = strtol(r_str, nullptr, 16);
        int g = strtol(g_str, nullptr, 16);
        int b = strtol(b_str, nullptr, 16);

        color = pixels.Color(r, g, b);
    }

    for (int i = 0; i < LED_COUNT; i++) {
        pixels.setPixelColor(i, color);
    }

    pixels.show();
}
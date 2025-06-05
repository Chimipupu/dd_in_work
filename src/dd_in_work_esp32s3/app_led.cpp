#include "app_led.hpp"

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static void led_test(void);
static void rgb_val_inc(rgb_led_t *p_led_t);

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
 * @brief RGB値のインクリメント関数
 * 
 * @param p_led_t RGBの色情報構造体ポインタ
 */
 static void rgb_val_inc(rgb_led_t *p_led_t)
{
    if (p_led_t->r < 255) {
        p_led_t->r++;
    } else if (p_led_t->g < 255) {
        p_led_t->g++;
    } else if (p_led_t->b < 255) {
        p_led_t->b++;
    } else {
        p_led_t->r = 0;
        p_led_t->g = 0;
        p_led_t->b = 0;
    }
}

/**
 * @brief LED色変更リクエストのデータ部分の生成関数
 * 
 * @param p_tx_buf 送信用データバッファポインタ
 * @param p_led_t RGBの色情報構造体ポインタ
 */
 void app_led_req_data_gen(uint8_t *p_tx_buf, rgb_led_t *p_led_t)
{
    char color_code[8] = {0}; // "#RRGGBB" + '\0' = 8文字

    // RGB値をインクリメント
    rgb_val_inc(p_led_t);

    // RGB値を "#RRGGBB" 形式に変換
    snprintf(color_code, sizeof(color_code),"#%02X%02X%02X",
            p_led_t->r, p_led_t->g, p_led_t->b);
    memcpy(p_tx_buf, color_code, strlen(color_code));
}

/**
 * @brief ASCIIで指定された色名、RGBt値をLEDに設定する関数
 * 
 * @param color_name RGBのASCII文字列（例:#FF32DB)
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
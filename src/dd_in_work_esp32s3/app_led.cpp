#include "app_led.hpp"

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static void led_test(void);

/**
 * @brief LEDアプリ初期化
 * 
 */
void app_led_init(void)
{
    // LEDを消灯
    pixels.Color(0, 0, 0);
}

/**
 * @brief LEDアプリメイン
 * 
 */
void app_led_main(void)
{
    // TODO
}

void app_led_set_color(String color_name)
{
    color_name.toLowerCase();  // 大文字小文字を区別しない

    uint32_t color;

    if (color_name == "red") { // 赤色
        color = pixels.Color(255, 0, 0);
    } else if (color_name == "green") { // 緑色
        color = pixels.Color(0, 255, 0);
    } else if (color_name == "blue") { // 青色
        color = pixels.Color(0, 0, 255);
    } else {
        color = pixels.Color(0, 0, 0);
    }

    for (int i = 0; i < LED_COUNT; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}
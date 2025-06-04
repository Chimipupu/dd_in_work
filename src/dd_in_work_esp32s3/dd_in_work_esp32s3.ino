/**
 * @file dd_in_work_esp32s3.ino
 * @author dd yf
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "app_espnow.hpp"
#include "app_led.hpp"

#if 1
// デバックでUARTでLEDの色を叩けるように
static void led_test(void)
{
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        app_led_set_color(input);
    }
}
#endif

void setup()
{
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    // LEDアプリ初期化
    app_led_init();

    // ESPNOWアプリ初期化
    app_esp_init();
}

void loop()
{
#if 0
    app_esp_main();
    delay(1000);
#else
    // (DEBUG)LEDのデバック
    led_test();
#endif
}
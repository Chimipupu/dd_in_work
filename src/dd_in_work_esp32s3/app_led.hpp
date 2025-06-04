#ifndef APP_LED_HPP
#define APP_LED_HPP

#include <stdint.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   48   // LEDとの接続ピン番号
#define LED_COUNT 1    // LEDの数

void app_led_set_color(String color_name);
void app_led_init(void);
void app_led_main(void);

#endif // APP_LED_HPP
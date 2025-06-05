#ifndef APP_LED_HPP
#define APP_LED_HPP

#include "common.hpp"
#include <Adafruit_NeoPixel.h>

#define LED_PIN   48   // LEDとの接続ピン番号
#define LED_COUNT 1    // LEDの数

/**
 * @brief RGBの色情報構造体
 * 
 */
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb_led_t;

void app_led_init(void);
void app_led_req_data_gen(uint8_t *p_tx_buf, rgb_led_t *p_led_t);
void app_led_set_color(String color_name);

#endif // APP_LED_HPP
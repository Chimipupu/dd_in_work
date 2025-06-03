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

void setup()
{
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    app_esp_init();
}

void loop()
{
    app_esp_main();
    delay(1000);
}
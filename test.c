/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"

#include "debug.h"
#include "rp2040-oled.h"

#include "hdc1080.h"

#include "source.h"

const uint hdc1080_sda_pin = 18;
const uint hdc1080_scl_pin = 19;

const uint sh1106_sda_pin = 4;
const uint sh1106_scl_pin = 5;

int main() {
    char buffer[24];

    stdio_init_all();

    sleep_ms(10000);

    DEBUG_printf("Start\n");
    sleep_ms(3000);

    sleep_ms(1000);

    rp2040_oled_t oled;
    oled.i2c = i2c0;
    oled.sda_pin = sh1106_sda_pin;
    oled.scl_pin = sh1106_scl_pin;
    oled.reset_pin = PIN_UNDEF;
    oled.baudrate = 400 * 1000;
    oled.invert = false;
    oled.flip = false;
    oled.size = OLED_128x64;
    oled.addr = 0x0;

    DEBUG_printf("Initialize\n");
    rp2040_oled_type_t type = rp2040_oled_init(&oled);
    DEBUG_printf("result = %d\n", type);
    sleep_ms(4000);

    DEBUG_printf("Initialize sensor\n");
    initialize_hdc1080(i2c1, hdc1080_sda_pin, hdc1080_scl_pin);

    uint16_t deviceId = hdc1080_read_device_id(i2c1);
    DEBUG_printf("DEVICE_ID: %x\n", deviceId);

    while(true) {

        // float x = (float)rand()/(float)(RAND_MAX/10);
        // float temp = 20.0 + x;
        // float humidity = 100.0 + x;
        float temp = hdc1080_temperature(i2c1);
        float humidity = hdc1080_humidity(i2c1);
        DEBUG_printf("Temperature: %.2f, humidity: %.1f\n", temp, humidity);

        rp2040_oled_clear(&oled);
        rp2040_oled_write_string(&oled, 10, 10, "Brutus's Sensor", 15, true);

        sprintf(buffer, "Temperature: %.1f", temp);
        rp2040_oled_write_string(&oled, 10, 24, buffer, strlen(buffer), true);

        sprintf(buffer, "Humidity: %.1f", humidity);
        rp2040_oled_write_string(&oled, 10, 40, buffer, strlen(buffer), true);

        sleep_ms(30000);
    }

    DEBUG_printf("Done\n");
}

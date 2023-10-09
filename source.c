/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"

#include "hdc1080.h"

#include "source.h"
#include "debug.h"

const uint hdc1080_sda_pin = 16;
const uint hdc1080_scl_pin = 17;

int main() {
    stdio_init_all();

    sleep_ms(10000);

    initialize_hdc1080(i2c0, hdc1080_sda_pin, hdc1080_scl_pin);
    sleep_ms(1000);

    if (cyw43_arch_init()) {
        DEBUG_printf("Wi-Fi init failed");
        return -1;
    }

    uint16_t deviceId = hdc1080_read_device_id(i2c0);
    DEBUG_printf("DEVICE_ID: %x\n", deviceId);

    while (true) {
        //DEBUG_printf("LED ON!\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(1000);
        //DEBUG_printf("LED OFF!\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(59000);

        float temp = hdc1080_temperature(i2c0);
        float humidity = hdc1080_humidity(i2c0);
        DEBUG_printf("Temperature: %.2f, humidity: %.1f\n", temp, humidity);
    }
}

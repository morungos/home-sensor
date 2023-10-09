#ifndef _HDC1080_H_
#define _HDC1080_H_

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

int initialize_hdc1080(i2c_inst_t *i2cn, uint sda_pin, uint scl_pin);

uint16_t hdc1080_read_device_id(i2c_inst_t *i2cn);

float hdc1080_temperature(i2c_inst_t *i2cn);
float hdc1080_humidity(i2c_inst_t *i2cn);

#endif // #ifndef _HDC1080_H_

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hdc1080.h"

static const uint8_t HDC1080_ADDRESS = 0x40;

static const uint8_t HDC1080_TEMPERATURE = 0x00;
static const uint8_t HDC1080_HUMIDITY = 0x01;
static const uint8_t HDC1080_CONFIGURATION = 0x02;
static const uint8_t HDC1080_MANUFACTURER_ID = 0xFE;
static const uint8_t HDC1080_DEVICE_ID = 0xFF;
static const uint8_t HDC1080_SERIAL_ID_FIRST = 0xFB;
static const uint8_t HDC1080_SERIAL_ID_MID = 0xFC;
static const uint8_t HDC1080_SERIAL_ID_LAST = 0xFD;

static const uint16_t DEVICE_ID = 0x1050;

static i2c_inst_t *i2c;

int reg_read(i2c_inst_t *i2c,
             const uint addr,
             const uint8_t reg,
             uint16_t delay,
             uint8_t *buf,
             const uint8_t nbytes) {
    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Read data from register(s) over I2C
    //printf("Selecting register: %x\n", reg);
    i2c_write_blocking(i2c, addr, &reg, 1, true);

    //printf("Sleep: %d\n", delay);
    sleep_ms(delay);

    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);
    //printf("Read response; bytes: %d\n", num_bytes_read);

    return num_bytes_read;
}

int reg_write(i2c_inst_t *i2c, 
              const uint addr, 
              const uint8_t reg, 
              uint8_t *buf,
              const uint8_t nbytes) {

    int num_bytes_read = 0;
    uint8_t msg[nbytes + 1];

    // Check to make sure caller is sending 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Append register address to front of data packet
    msg[0] = reg;
    for (int i = 0; i < nbytes; i++) {
        msg[i + 1] = buf[i];
    }

    // Write data to register(s) over I2C
    i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

    return num_bytes_read;
}

int initialize_hdc1080(i2c_inst_t *i2cn, uint sda_pin, uint scl_pin) {
    // Store the controller
    i2c = i2cn;

    // Initialize it
    printf("Initializing I2C\n");
    i2c_init(i2c, 400 * 1000);

    printf("Configuring GPIO pins\n");
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);

    printf("All set\n");
}

static uint16_t hdc1080_read_register(i2c_inst_t *i2cn, uint8_t reg, uint16_t delay) {
    uint8_t data[2];

    int bytes = reg_read(i2cn, HDC1080_ADDRESS, reg, delay, data, 2);
    //printf("hdc1080_read_register: register: %x, bytes: %d\n", reg, bytes);
    if (bytes != 2) {
        return 0;
    }
    //printf("hdc1080_read_register: data: %x, %x\n", data[0], data[1]);
    return (data[0] << 8) + data[1];
}

uint16_t hdc1080_read_device_id(i2c_inst_t *i2cn) {
    uint16_t result;
    result = hdc1080_read_register(i2cn, HDC1080_DEVICE_ID, 10);
    //printf("HDC1080_DEVICE_ID: %d\n", result);
    return result;
}

float hdc1080_temperature(i2c_inst_t *i2cn) {
    uint16_t result;
    
    result = hdc1080_read_register(i2cn, HDC1080_TEMPERATURE, 150);
    //printf("HDC1080_TEMPERATURE: %d\n", result);
    return ((float) result / 65536.0f) * 165.0f - 40.0f;
}

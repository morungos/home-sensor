# What CMake to start at
cmake_minimum_required(VERSION 3.25)

set(PICO_BOARD pico_w)
 
# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)

add_compile_options(-Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
)
 
# Set the name and version of the project
project(PicoTest VERSION 1.0.0 LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID")
set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi password")

file(READ "certs/ca.crt" FILE_CRYPTO_CA)
file(READ "certs/client.crt" FILE_CRYPTO_CLIENT)
file(READ "certs/client.key" FILE_CRYPTO_CLIENT_KEY)

file(READ "certs/mqtt-user.txt" FILE_MQTT_USER)
file(READ "certs/mqtt-password.txt" FILE_MQTT_PASSWORD)
file(READ "certs/mqtt-server.txt" FILE_MQTT_SERVER)

# Link the Project to a source file (step 4.6)
# add_executable(PicoTest source.c hdc1080.c)
add_executable(PicoTest picow_iot.c)

target_compile_definitions(PicoTest PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
    FILE_CRYPTO_CA=\"${FILE_CRYPTO_CA}\"
    FILE_CRYPTO_CLIENT=\"${FILE_CRYPTO_CLIENT}\"
    FILE_CRYPTO_CLIENT_KEY=\"${FILE_CRYPTO_CLIENT_KEY}\"
    FILE_MQTT_USER=\"${FILE_MQTT_USER}\"
    FILE_MQTT_PASSWORD=\"${FILE_MQTT_PASSWORD}\"
    FILE_MQTT_SERVER=\"${FILE_MQTT_SERVER}\"
    NO_SYS=1
)

target_include_directories(PicoTest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)
 
# Link the Project to an extra library (pico_stdlib)
target_link_libraries(PicoTest 
    pico_stdlib 
    pico_cyw43_arch_lwip_poll
    pico_lwip_mbedtls
    pico_lwip_mqtt
    pico_mbedtls
    hardware_i2c
)
 
# Initalise the SDK
pico_sdk_init()
 
# Enable USB, UART output
pico_enable_stdio_usb(PicoTest 1)
pico_enable_stdio_uart(PicoTest 1)
 
# Enable extra outputs (SWD?)
pico_add_extra_outputs(PicoTest)


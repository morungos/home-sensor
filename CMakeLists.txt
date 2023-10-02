# What CMake to start at
cmake_minimum_required(VERSION 3.25)

set(PICO_BOARD pico_w)
 
# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)
 
# Set the name and version of the project
project(PicoTest VERSION 1.0.0 LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
 
# Link the Project to a source file (step 4.6)
add_executable(PicoTest source.c hdc1080.c)
 
# Link the Project to an extra library (pico_stdlib)
target_link_libraries(PicoTest 
    pico_stdlib 
    pico_cyw43_arch_none
    hardware_i2c
)
 
# Initalise the SDK
pico_sdk_init()
 
# Enable USB, UART output
pico_enable_stdio_usb(PicoTest 1)
pico_enable_stdio_uart(PicoTest 1)
 
# Enable extra outputs (SWD?)
pico_add_extra_outputs(PicoTest)


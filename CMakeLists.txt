cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(suprglidr)
pico_sdk_init()

add_executable(suprglidr
    src/debug.c
    src/output.c
    src/input.c
    src/main.c
    src/util.c
    src/ssd1306.c
)

pico_generate_pio_header(suprglidr src/enc.pio)

pico_enable_stdio_usb(suprglidr 1)
pico_enable_stdio_uart(suprglidr 0)
target_link_libraries(suprglidr
    pico_stdlib
    hardware_pwm
    pico_time
    pico_rand
    pico_stdio
    pico_sync
    hardware_i2c
    pico_multicore
    hardware_pio
)
pico_add_extra_outputs(suprglidr)


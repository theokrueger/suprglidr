cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(suprglidr)

set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

pico_sdk_init()

add_compile_options(-Wall)

add_executable(suprglidr
    src/debug.c
    src/output.c
    src/input.c
    src/main.c
    src/util.c
    src/ssd1306.c
    src/usb_descriptors.c
    src/keyboard.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(suprglidr PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)

pico_generate_pio_header(suprglidr ${CMAKE_CURRENT_LIST_DIR}/src/encoder.pio)

pico_enable_stdio_usb(suprglidr 0)
pico_enable_stdio_uart(suprglidr 0)

target_link_libraries(suprglidr
    pico_stdlib
    pico_time
    pico_rand
    pico_stdio
    pico_sync
    hardware_i2c
    pico_multicore
    hardware_pio
    hardware_irq
    tinyusb_device
    tinyusb_board
)
pico_add_extra_outputs(suprglidr)


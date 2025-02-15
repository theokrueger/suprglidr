#pragma once

#include "pico/stdlib.h"


void setup_output();

/* info screen */
#define INFO_SCREEN_SDA 0
#define INFO_SCREEN_SCL 1
#define INFO_SCREEN_ADDRESS 0x3c
#define INFO_SCREEN_WIDTH 128
#define INFO_SCREEN_HEIGHT 64
#define INFO_SCREEN_I2C_INSTANCE i2c0

void set_info_text(char *line1, char *line2, char *line3);

void clear_text();

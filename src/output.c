//! output.c
//!
//! Handles LED and shock collar outputs
//! Functions for displaying lives and such

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include "hardware/i2c.h"

#include "../include/output.h"
#include "../include/input.h"
#include "../include/util.h"
#include "../include/ssd1306.h"

const int INFO_CHAR_SCALE = 2;
const int INFO_LINE_HEIGHT = INFO_SCREEN_HEIGHT / 3;

ssd1306_t info_screen;

void set_info_text(char *line1, char *line2, char *line3)
{
	printf("Setting text to:\n\n%s\n%s\n%s\n\n", line1, line2, line3);
        clear_text();
	ssd1306_draw_string(&info_screen, 0, 0, INFO_CHAR_SCALE, line1);
	ssd1306_draw_string(&info_screen, 0, INFO_LINE_HEIGHT, INFO_CHAR_SCALE, line2);
	ssd1306_draw_string(&info_screen, 0, 2*INFO_LINE_HEIGHT, INFO_CHAR_SCALE, line3);
	ssd1306_show(&info_screen);
}

void clear_text()
{
	ssd1306_clear(&info_screen);
}

void setup_output()
{
	// i2c
	i2c_init(INFO_SCREEN_I2C_INSTANCE, 100 * 1000); // 100khz baud
	gpio_set_function(INFO_SCREEN_SDA, GPIO_FUNC_I2C);
	gpio_set_function(INFO_SCREEN_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(INFO_SCREEN_SDA);
	gpio_pull_up(INFO_SCREEN_SCL);

	// setup screen
	info_screen.external_vcc=false;
	ssd1306_init(&info_screen, INFO_SCREEN_WIDTH, INFO_SCREEN_HEIGHT, INFO_SCREEN_ADDRESS, INFO_SCREEN_I2C_INSTANCE);
	ssd1306_clear(&info_screen);
}

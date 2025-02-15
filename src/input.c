//! input.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"

#include "../include/input.h"
#include "../include/output.h"

void setup_input()
{
	printf("Setting up input\n");

	// confirm
	gpio_init(BTN_CONFIRM);
	gpio_set_dir(BTN_CONFIRM, GPIO_IN);
	gpio_set_pulls(BTN_CONFIRM, true, false); // set as pull up

	// keyswitch
	gpio_init(BTN_SWITCH);
	gpio_set_dir(BTN_SWITCH, GPIO_IN);
	gpio_set_pulls(BTN_SWITCH, true, false); // set as pull up
}

int switch_is_pressed()
{
	return !gpio_get(BTN_SWITCH);
}

int confirm_is_pressed()
{
	return !gpio_get(BTN_CONFIRM);
}

int encoder_state()
{
	return ENC_NONE;
}

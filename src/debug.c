//! debugging functions

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include "hardware/watchdog.h"

#include "../include/input.h"
#include "../include/output.h"
#include "../include/debug.h"
#include "../include/util.h"


void debug_init()
{
	// run debug stuff only when trigger is help
	sleep_ms(3000);
	if (watchdog_caused_reboot()) {
		printf("Rebooted by Watchdog!\n");
	} else {
		printf("Clean boot\n");
	}
	test_all_io();
}

void test_btn(char *name, int gpio, int state_func())
{
	char buf[15];
	sprintf(buf, "(gpio: %i)", gpio);
	set_info_text("PRESS:", name, buf);

	while (!state_func());

	set_info_text("", "SUCCESS", "");
	sleep_ms(250);
	return;
}

void test_enc(char *name, int target, int state_func())
{
	char buf[15];
	sprintf(buf, "(trgt: %i)", target);
	set_info_text("ROTATE:", name, buf);

	while (state_func() != target);

	set_info_text("", "SUCCESS", "");
	sleep_ms(250);
	return;
}

/// test all currently implemented io functions
void test_all_io()
{
	// intro
	printf("Testing i/o");
	set_info_text("welcome to", "i/o test", "pls wait");
	sleep_ms(1000);

        test_btn("switch", BTN_SWITCH, switch_is_pressed);

	// test encoder
	test_enc("encCCW", ENC_CCW, encoder_state);
	test_enc("encCW", ENC_CW, encoder_state);
	test_btn("confirm", BTN_CONFIRM, confirm_is_pressed);
}

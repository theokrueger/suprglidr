//! suprglidr
//!
//! small accessory to press buttons for you
//! made for the game Apex Legends

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/sync.h"
#include "hardware/watchdog.h"
#include "pico/multicore.h"

#include "../include/input.h"
#include "../include/output.h"
#include "../include/debug.h"
#include "../include/util.h"

void run_settings();
void empty_routine();
void change_screen(int screen);
void fps_routine();
void deviation_routine();
void jump_routine();
void crouch_routine();
int wrap(int val, int min, int max);

int main()
{
	// init
	stdio_init_all();
	printf("Starting...\n");
	setup_input();
	setup_output();
	debug_init();

        // main loop
	multicore_launch_core1(&run_settings);
	//run_settings();
	while(1);

	// reset the pico in event loop ends
	watchdog_enable(1, 1);
	while(1);
}


// app
#define SCREENS 5
char *SCREEN_TEXT[SCREENS] = {
	"suprglidr.",
	"set FPS",
	"set dev.",
	"set jump",
	"set crouch",
};
typedef void (*routine)(void);
const routine SCREEN_ROUTINES[SCREENS] = {
	&empty_routine,
	&fps_routine,
	&deviation_routine,
	&jump_routine,
	&crouch_routine
};

int cur_screen = 0;

void run_settings()
{
	change_screen(cur_screen);
	int last_screen = cur_screen;
	while(1)
	{
	        if(confirm_is_pressed()) {
			sleep_ms(200);
			SCREEN_ROUTINES[cur_screen]();
			last_screen = -1;
		}

		int state = encoder_state();
	        cur_screen += state; // +/- 1
	        cur_screen = wrap(cur_screen, 0, SCREENS - 1);

		if(last_screen != cur_screen) {
			change_screen(cur_screen);
			clear_enc_state();
		}

		last_screen = cur_screen;
	}
}

int wrap(int val, int min, int max)
{
	if (val < min) return max;
	else if (val > max) return min;

	return val;
}

void change_screen(int screen)
{
	set_info_text("", SCREEN_TEXT[screen], "");
}

void empty_routine()
{
	return;
}

#define FPS_CNT
const int FPS[FPS_CNT] = {60, 120, 144, 200, 240};
void fps_routine()
{
	int sel = 0;
	while(1)
	{
		if(confirm_is_pressed()) return;
		char buf[5];
		sprintf(buf, "%i", FPS[sel]);
		set_info_text("Set FPS:", buf, "");

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, 0, FPS_CNT - 1);
	}
}

const int DEV_MIN = 0;
const int DEV_MAX = 100;
void deviation_routine()
{
	int sel = 0;
	while(1)
	{
		if(confirm_is_pressed()) return;
		char buf[5];
		sprintf(buf, "%i", sel);
		set_info_text("Set dev.:", buf, "");

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, DEV_MIN, DEV_MAX);
	}
}

void jump_routine()
{

}

void crouch_routine()
{

}

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
#include "hardware/pio.h"

#include "../include/input.h"
#include "../include/output.h"
#include "../include/debug.h"
#include "../include/util.h"

void run_settings();
void empty_routine();
void change_screen(int screen);

int main()
{
	// init
	stdio_init_all();
	printf("Starting...\n");
	setup_input();
	setup_output();
	debug_init();

        // main loop
	//multicore_launch_core1(&run_settings);
	run_settings();
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
	"set deviation",
	"set jump",
	"set crouch",
};
typedef void (*routine)(void);
const routine SCREEN_ROUTINES[SCREENS] = {
	&empty_routine,
	&empty_routine,
	&empty_routine,
	&empty_routine,
	&empty_routine
};

int cur_screen = 0;

void run_settings()
{
	change_screen(cur_screen);
	while(1)
	{
	        cur_screen += encoder_state(); // +/- 1
	        if (cur_screen < 0) cur_screen = SCREENS - 1;
		else if (cur_screen >= SCREENS) cur_screen = 0;

		if(confirm_is_pressed()) SCREEN_ROUTINES[cur_screen]();
	}
}

void change_screen(int screen)
{
	set_info_text("", SCREEN_TEXT[screen], "");
}

void empty_routine()
{
	return;
}

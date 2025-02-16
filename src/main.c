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
#include "bsp/board.h"
#include "tusb.h"

#include "../include/input.h"
#include "../include/output.h"
#include "../include/debug.h"
#include "../include/util.h"
#include "../include/usb_descriptors.h"
#include "../include/keyboard.h"

void run_settings();
void run_keyboard();
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
        run_keyboard();

	// reset the pico in event loop ends
	watchdog_enable(1, 1);
	while(1);
}

// keyboard
static void send_hid_report(bool keys_pressed)
{
	uint8_t crouch_code[6] = {get_crouch(), 0};
	uint8_t jump_code[6] = {get_jump(), 0};
	if (!tud_hid_ready())
	{
		return;
	}

	static bool send_empty = false;

	if (keys_pressed)
	{
		tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, jump_code);
		send_empty = true;
	}
	else
	{
		if (send_empty)
		{
			tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
		}
		send_empty = false;
	}
}

void hid_task(void)
{
	// poll every 10ms
	const uint32_t interval_ms = 5;
	static uint32_t start_ms = 0;

	if (board_millis() - start_ms < interval_ms)
	{
		return; // not enough time
	}
	start_ms += interval_ms;

	// Check for keys pressed
	bool const keys_pressed = update();

	send_hid_report(keys_pressed);
}

// stubs
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {}
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {}
void tud_mount_cb(void) {}
void tud_umount_cb(void) {}
void tud_suspend_cb(bool remote_wakeup_en) {}
void tud_resume_cb(void) {}

void run_keyboard()
{
        board_init();
        tusb_init();
	while(1)
	{
		tud_task();
		hid_task();
	}
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
			sleep_ms(200);
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

#define FPS_CNT 5
const int FPS[FPS_CNT] = {60, 120, 144, 200, 240};
void fps_routine()
{
	int sel = 0;
	int last = -1;
	int cur = get_fps();
	while(1)
	{
		printf("%i\n", sel);
		if(confirm_is_pressed()) return;

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, 0, FPS_CNT - 1);

		if(sel != last) {
			char sel_s[10];
			sprintf(sel_s, "%i%%", sel);
			char cur_s[10];
			sprintf(cur_s, "cur: %i%%", cur);
			set_info_text("set FPS:", sel_s, cur_s);
		}

		last = sel;
	}
}

const int DEV_MIN = 0;
const int DEV_MAX = 100;
void deviation_routine()
{
	int sel = 0;
	int last = -1;
	int cur = get_deviation();
	while(1)
	{
		if(confirm_is_pressed()) return;

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, DEV_MIN, DEV_MAX);

		if(sel != last) {
			char sel_s[10];
			sprintf(sel_s, "%i%%", sel);
			char cur_s[10];
			sprintf(cur_s, "cur: %i%%", cur);
			set_info_text("set dev.:", sel_s, cur_s);
		}

		last = sel;
	}
}

#define JUMPBIND_CNT 4
const char *JUMPBIND[JUMPBIND_CNT] = {"<SPACE>", "<CTRL>", "V", "B"};
void jump_routine()
{
	int sel = 0;
	int last = -1;
	int cur = get_jump();
	while(1)
	{
		printf("%i\n", sel);
		if(confirm_is_pressed()) return;

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, 0, JUMPBIND_CNT - 1);

		if(sel != last) {
			char buf[15];
			sprintf(buf, "%s", JUMPBIND[sel]);
			char cur_s[10];
			sprintf(cur_s, "cur: %i%%", cur);
			set_info_text("set jump:", buf, cur_s);
		}

		last = sel;
	}
}


#define CROUCHBIND_CNT 4
const char *CROUCHBIND[JUMPBIND_CNT] = {"<SPACE>", "<CTRL>", "V", "B"};
void crouch_routine()
{
	int sel = 0;
	int last = -1;
	int cur = get_crouch();
	while(1)
	{
		printf("%i\n", sel);
		if(confirm_is_pressed()) return;

		int state = encoder_state();
	        sel += state; // +/- 1
	        sel = wrap(sel, 0, CROUCHBIND_CNT - 1);

		if(sel != last) {
			char buf[15];
			sprintf(buf, "%s", CROUCHBIND[sel]);
			char cur_s[10];
			sprintf(cur_s, "cur: %i%%", cur);
			set_info_text("set jump:", buf, cur_s);
		}

		last = sel;
	}
}

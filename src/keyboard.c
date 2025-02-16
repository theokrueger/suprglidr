#include "class/hid/hid.h"

#include "../include/input.h"

int last = 0;
int update()
{
	int state = switch_is_pressed();
	int changed = state != last;
	last = state;
	return changed;
}

int fps = 240;
void set_fps(int val)
{
	fps = val;
}

int deviation = 50;
void set_deviation(int val)
{
	deviation = val;
}

uint8_t jump = HID_KEY_SPACE;
void set_jump(uint8_t val)
{
	jump = val;
}

uint8_t crouch = HID_KEY_V;
void set_crouch(uint8_t val)
{
	crouch = val;
}


int get_fps()
{
	return fps;
}

int get_deviation()
{
	return deviation;
}

uint8_t get_jump()
{
	return jump;
}

uint8_t get_crouch()
{
	return crouch;
}

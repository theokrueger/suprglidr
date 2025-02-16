#pragma once
#include "pico/stdlib.h"

int update();

void set_fps(int val);

void set_deviation(int val);

void set_jump(uint8_t val);

void set_crouch(uint8_t val);

int get_fps();

int get_deviation();

uint8_t get_jump();

uint8_t get_crouch();

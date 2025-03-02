#pragma once

void setup_input();

// encoder states
#define ENC_CW 1
#define ENC_CCW -1
#define ENC_NONE 0

// main input switch (other side should be GND)
#define BTN_SWITCH 14
int switch_is_pressed();

// encoder switch
#define BTN_CONFIRM 27
int confirm_is_pressed();

// encoder knob
#define ENC_A 28
#define ENC_B 29
int encoder_state();

// clear encoder state
void clear_enc_state();

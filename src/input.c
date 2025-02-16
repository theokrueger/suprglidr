//! input.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include "../include/input.h"
#include "../include/output.h"
#include "encoder.pio.h"

int enc_state = ENC_NONE;
void irq_handler()
{
        if (pio0_hw->irq & 1)
        {
		enc_state = ENC_CW;
        }
        if (pio0_hw->irq & 2)
        {
		enc_state = ENC_CCW;
        }
        pio0_hw->irq = 3;
}

void setup_input()
{
	printf("Setting up input\n");

	// confirm
	gpio_init(BTN_CONFIRM);
	gpio_set_dir(BTN_CONFIRM, GPIO_IN);
	gpio_set_pulls(BTN_CONFIRM, true, false);

	// keyswitch
	gpio_init(BTN_SWITCH);
	gpio_set_dir(BTN_SWITCH, GPIO_IN);
	gpio_set_pulls(BTN_SWITCH, true, false);

	// encoder
	PIO pio = pio0;
	uint sm = 0;
	uint offset ;
	encoder_program_init(pio, sm, &offset, ENC_A, ENC_B, irq_handler);
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
	int state = enc_state;
	enc_state = ENC_NONE;
	sleep_ms(50);
	return state;
}

void clear_enc_state()
{
	enc_state = ENC_NONE;
}

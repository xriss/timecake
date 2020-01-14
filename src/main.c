
#include "sys/debug.h"

#include "sys/saveram.h"
#include "sys/acc.h"
#include "sys/heart.h"
#include "sys/touch.h"
#include "sys/battery.h"
#include "sys/lcd.h"
#include "sys/clock.h"
#include "sys/button.h"

#include "nrf.h"
#include "nrf_gpio.h"

#include "main.h"

// hardware reference manual
// https://infocenter.nordicsemi.com/pdf/nRF52832_OPS_v0.6.3.pdf

int main_butt=0;

char main_text[32*16];
struct shader_font main_lines[16];


int main_state=0;
int main_state_next=2;

int main_state_call(int mode)
{
	switch(main_state)
	{
		case 0:
			switch(mode)
			{
				case 1: // setup, screen off
					lcd_backlight(0);
					lcd_sleep(255);
				break;
				case 2: // sleepy update
					__SEV();
					__WFE();
					__WFE();
				break;
				case 3: // clean, screen on
					lcd_sleep(0);
					lcd_backlight(255);
				break;
			}
		break;
		case 1: return main_test(mode);
		case 2: return main_clock1(mode);
	}
	return 0;
}

int main(void)
{
	saveram_setup();

	acc_setup();
	heart_setup();
	touch_setup();
	battery_setup();
	lcd_setup();
	clock_setup();
	button_setup();
	
	while(1)
	{
		main_butt = button_read();
		if( main_butt & 2 ) // pressed down
		{
			main_state_next=main_state+1;
		}

		if(main_state_next) // flag a state change 
		{
			lcd_backlight(0); // display off
			
			main_state_call(3); // clean old state

			main_state=main_state_next;
			main_state_next=0;

			if(main_state>2) { main_state=0; } // max state wrap, state 0 is sleepy time

			main_state_call(1); // setup new state
		}

		
		main_state_call(2); // update current state

		lcd_backlight(255); // display on
	}

	return 0;
}

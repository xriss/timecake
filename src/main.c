
#include "sys/debug.h"

#include "sys/saveram.h"
#include "sys/acc.h"
#include "sys/heart.h"
#include "sys/touch.h"
#include "sys/battery.h"
#include "sys/lcd.h"
#include "sys/clock.h"
#include "sys/button.h"

#include "main.h"

// hardware reference manual
// https://infocenter.nordicsemi.com/pdf/nRF52832_OPS_v0.6.3.pdf


char main_text[32*16];
struct shader_font main_lines[16];


int main_state=0;
int main_state_next=1;

int main_state_call(int mode)
{
	switch(main_state)
	{
		case 1: return main_test(mode);
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
		if(main_state_next) // flag a state change 
		{
			main_state_call(3); // clean old state

			main_state=main_state_next;
			main_state_next=0;

			main_state_call(1); // setup new state
		}

		main_state_call(2); // update current state
	}

	return 0;
}


// pine button code

#include <nrf.h>
#include "nrf_gpio.h"

#include "button.h"
#include "saveram.h"

#define BUTTON_PIN_IN  13
#define BUTTON_PIN_OUT 15

int button_state=0;

int button_setup(void)
{
	nrf_gpio_cfg_output(BUTTON_PIN_OUT);
    nrf_gpio_cfg_sense_input(BUTTON_PIN_IN, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
	nrf_gpio_pin_write(BUTTON_PIN_OUT,1);


	NVIC_EnableIRQ(GPIOTE_IRQn);
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Msk;
	NRF_GPIOTE->CONFIG[0] =
		(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	|
		(BUTTON_PIN_IN << GPIOTE_CONFIG_PSEL_Pos)						|
		(GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos)			;

	return 0;
}

// this reads and clears DOWN/UP flags
int button_read(void)
{
	int b=button_state;
	button_state&=0x1; // clear state change flags ( very small chance to miss a button press here :( )
	return b;
}

static unsigned int debounce1=0;
static unsigned int debounce2=0;

void GPIOTE_IRQHandler(void)
{
	if(NRF_GPIOTE->EVENTS_IN[0]) // toggle
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0;

		// this might be unsafe?
		int b=nrf_gpio_pin_read(BUTTON_PIN_IN);
		
		unsigned int d1=saveram->clock;
		unsigned int d2=NRF_RTC0->COUNTER;
		int nobounce=0;
		if( debounce1+1 < d1 ) { nobounce=1; } // require more than one sec
		else
		if( debounce1 > d1 ) { nobounce=1; } // ERROR reset bounce counters
		else // check ticks
		{
			d2+=(d1-debounce1)*0x8000; // add secs
			if( (d2-debounce2) > 0x0080) { nobounce=1; } // require more than 1/64 of a sec between state changes
		}

		if(b)
		{
			
			if( nobounce && ((button_state&1)==0) ) { button_state|=2; }
			button_state|=1;
		}
		else
		{
			if( nobounce && ((button_state&1)==1) ) { button_state|=4; }
			button_state&=6;
		}
		
		if(nobounce)
		{
			debounce1=(unsigned int)saveram->clock; // i think this double access is safe in interrupt?
			debounce2=NRF_RTC0->COUNTER; // anyway it will mostly work
		}


	}


}


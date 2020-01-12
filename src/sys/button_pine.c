
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

static unsigned int debounce=0;

void GPIOTE_IRQHandler(void)
{
	if(NRF_GPIOTE->EVENTS_IN[0]) // toggle
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0;

		// disable for safety?
//		NRF_GPIOTE->CONFIG[0] = 0;

		// aparently this is unsafe?
		int b=nrf_gpio_pin_read(BUTTON_PIN_IN);
		
		int safe=0;
		if(debounce!=saveram->clock) { safe=1; } // probably more than one sec

		if(b)
		{
			button_state|=1;
			if(safe) { button_state|=2; }
		}
		else
		{
			button_state&=6;
			if(safe) { button_state|=4; }
		}
		
		if(safe)
		{
			debounce=(int)saveram->clock;
		}

		// enable
//		NRF_GPIOTE->CONFIG[0] =
//			(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	|
//			(BUTTON_PIN_IN << GPIOTE_CONFIG_PSEL_Pos)						|
//			(GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos)			;

	}


}


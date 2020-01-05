
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

int main(void)
{
	while(1)
	{
		nrf_gpio_pin_toggle(22);
		nrf_gpio_pin_toggle(23);
		nrf_gpio_pin_toggle(14);
		nrf_delay_ms(500);
	}

}

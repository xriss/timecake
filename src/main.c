
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

int main(void)
{
	
	nrf_gpio_cfg_output(22);
	nrf_gpio_cfg_output(23);
	nrf_gpio_cfg_output(14);
	
	
	while(1)
	{
		nrf_gpio_pin_write(22,0);
		nrf_gpio_pin_write(23,0);
		nrf_gpio_pin_write(14,0);
		nrf_delay_ms(500);
		nrf_gpio_pin_write(22,1);
		nrf_gpio_pin_write(23,1);
		nrf_gpio_pin_write(14,1);
		nrf_delay_ms(500);
	}

}

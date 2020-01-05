
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#include "sys/lcd.h"


int main(void)
{

	lcd_setup();


	while(1)
	{
		for(int i=0;i<256;i++)
		{
			lcd_backlight(i);
			nrf_delay_ms(10);
		}
	}

}

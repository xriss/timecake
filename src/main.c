
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
			int d=i>128?256-i:i;
			lcd_backlight(d+64+32);
			nrf_delay_ms(10);
		}
	}

}

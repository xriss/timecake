
// pine lcd code

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#define LCD_BACKLIGHT_LOW 24
#define LCD_BACKLIGHT_MID 22
#define LCD_BACKLIGHT_HIGH 23

int lcd_setup(void)
{
	
	nrf_gpio_cfg_output(LCD_BACKLIGHT_LOW);
	nrf_gpio_cfg_output(LCD_BACKLIGHT_MID);
	nrf_gpio_cfg_output(LCD_BACKLIGHT_HIGH);
	
	return 0;
}


/*
Only 4 levels but use a bright value from 0 to 255

0x00 == off
0x40 == low
0x80 == medium
0xc0 == high

*/
int lcd_backlight(int bright)
{

	nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,1);
	nrf_gpio_pin_write(LCD_BACKLIGHT_MID,1);
	nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,1);

	if(bright<0x40) // darkest
	{
	}
	else
	if(bright<0x80)
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,0);
	}
	else
	if(bright<0xc0)
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_MID,0);
	}
	else
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,0);
	}

	return 0;
}

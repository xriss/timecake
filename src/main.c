
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#include "sys/lcd.h"


static int shader_test(int x,int y,void *data)
{
	int d=*((int*)data);
	int dd=d*d;
	int r=0x000010;
	int cx=x-120;
	int cy=y-120;
	int cc=( cx*cx + cy*cy );
	if( cc < dd )
	{
		r=(0x00ff00-(((cc<<16)/dd)&0x00ff00))|0xff0000;
	}
	return r;
}


int main(void)
{

	lcd_setup();

	struct shader_font line[1];
	
	line->name=funfont_8x16r;
	line->hx=8;
	line->hy=16;
	line->foreground=0xffffff;
	line->background=0x000010;
	line->text="Hello World!";
	line->length=strlen(line->text);
	line->ax=0;line->mx=1;line->dx=2;
	line->ay=0;line->my=1;line->dy=2;

	int i=1;
	int j=1;
	int k=1;
	while(1)
	{
		if(k>0)
		{
			i=i+j;
			j=j+1;
		}
		else
		{
			j=j-1;
			if(j<=0) { j=1; }
			i=i-j;
		}
		
		if(i>=120)
		{
			k=-1;
		}
		else
		if(i<=0)
		{
			k=1;
		}
		else
		{
			lcd_shader(0,32,240,240-32,shader_test,&i);
			lcd_shader(0,0 ,240,32    ,shader_textline,line);
		}
		
/*
		for(int i=0;i<256;i++)
		{
			int d=i>128?256-i:i;
			lcd_backlight(d+64+32);
			nrf_delay_ms(10);
		}
*/

	}

}

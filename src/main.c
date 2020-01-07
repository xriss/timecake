
#include <time.h>
#include <string.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#include "sys/battery.h"
#include "sys/lcd.h"


static char text[32*16];
static struct shader_font lines[16];

static int shader_test(int _x,int _y,void *data)
{
	int x=239-_y; // rotate screen
	int y=_x;

	int r=-1; // background flag

	int ty=(y/16)&0xf; // 0-15 line
	struct shader_font *line=lines+ty;
	if(line->length>0) // render some text
	{
		r=shader_textline(x,y,line);
	}

	
	if(r==-1) // fill background with simple animation
	{
		int d=*((int*)data);
		int dd=d*d;
		int cx=x-120;
		int cy=y-120;
		int cc=( cx*cx + cy*cy );
		if( cc < dd )
		{
			r=(0x00ff00-(((cc<<16)/dd)&0x00ff00))|0xff0000;
		}
		else
		{
			r=0x000010;
		}

	}
	return r;
}


int main(void)
{
	int idx;
	
	battery_setup();
	lcd_setup();
	
	// setup text screen buffers for a 30x15 character display.
	for(idx=0;idx<16;idx++)
	{
		lines[idx].name=funfont_8x16r;
		lines[idx].hx=8;
		lines[idx].hy=16;
		lines[idx].foreground=0xffffff;
		lines[idx].background=-1; // transparent
		lines[idx].ax=0;lines[idx].mx=1;lines[idx].dx=1;
		lines[idx].ay=0;lines[idx].my=1;lines[idx].dy=1;
		
		lines[idx].ay=-(idx*16);

		lines[idx].text=text+(idx*32); // 32 char buffers per line (30 visible with 8x16 font)
		lines[idx].length=0;
	}

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
			int flags;
			float voltage;
			float percent;
			battery_read(&flags,&voltage,&percent);

			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			snprintf(lines[0].text,32,"Hello World!");
			snprintf(lines[1].text,32,"Battery : %d.%03dv : %3d%%",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent);
			snprintf(lines[2].text,32,"%d-%02d-%02d %02d:%02d:%02d", tm->tm_year+1900 , tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec );

			for(idx=0;idx<16;idx++) { lines[idx].length=strlen(lines[idx].text); }

			lcd_shader(0,0,240,240,shader_test,&i);
    
		}

	}

}


#include <time.h>
#include <string.h>
#include <math.h>

#include <nrf.h>
#include "nrf_gpio.h"

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

static int frame=0;
static struct tm *clocks=0;



static int shader_test(int _x,int _y,void *data)
{
	int x=239-_y; // rotate screen
	int y=_x;

	int r=-1; // background flag

	int ty=(y/16)&0xf; // 0-15 line
	struct shader_font *line=main_lines+ty;
	if(line->length>0) // render some text
	{
		r=shader_textline(x,y,line);
	}

	
	if(r==-1) // fill background with simple animation
	{
		int d=120;
		int dd=d*d;
		float cx=x-120.0f;
		float cy=y-120.0f;
		float cc=( cx*cx + cy*cy );
		if( cc < dd )
		{
			r=0x000010;

			float t=(atan2f(-cx,cy)/(M_PI*2.0f))+0.5f;
			float c=sqrtf(cc)/120.0f;
			
			if(c<0.2f)
			{
				if( t*365.0f < clocks->tm_yday )
				{ r=0x444444; }
			}
			else
			if(c<0.4f)
			{
				if( t*31.0f < clocks->tm_mday )
				{ r=0x666666; }
			}
			else
			if(c<0.6f)
			{
				if( t*24.0f < clocks->tm_hour )
				{ r=0x888888; }
			}
			else
			if(c<0.8f)
			{
				if( t*60.0f < clocks->tm_min )
				{ r=0xaaaaaa; }
			}
			else
			{
				if( t*60.0f < clocks->tm_sec )
				{ r=0xcccccc; }
			}
		}
		else
		{
			r=0x000010;
		}

	}
	return r;
}


static int main_setup()
{
	// setup text screen buffers for a 30x15 character display.
	for(int idx=0;idx<16;idx++)
	{
		struct shader_font *line=main_lines+idx;
		line->name=funfont_8x16r;
		line->hx=8;
		line->hy=16;
		line->foreground=0xffffff;
		line->dropshadow=0x000000;
		line->background=-1; // transparent
		line->ax=0;line->mx=1;line->dx=1;
		line->ay=0;line->my=1;line->dy=1;
		
		line->ay=-(idx*16);

		line->text=main_text+(idx*32); // 32 char buffers per line (30 visible with 8x16 font)
		line->text[0]=0;
		line->length=0;
	}
	return 0;
}

static int main_clean()
{
	return 0;
}

static int main_update()
{

	time_t t16 = clock_time(); // seconds since 1970 * 65536
	time_t t = t16>>16; // seconds since 1970
	clocks = localtime(&t);

	int flags;
	float voltage;
	float percent;
	battery_read(&flags,&voltage,&percent);
	char * charging="    ";
//			if(flags==2) { charging="===="; } // Charged (never reaches this state?)
//			else
	if(flags&1) // Charging
	{
		switch(frame&3) // animate
		{
			case 0: charging="+   "; break;
			case 1: charging="++  "; break;
			case 2: charging="+++ "; break;
			case 3: charging="++++"; break;
		}
	}
	else // Discharging
	{
		switch(frame&3) // animate
		{
			case 0: charging="----"; break;
			case 1: charging="--- "; break;
			case 2: charging="--  "; break;
			case 3: charging="-   "; break;
		}
	}

	int idx=0;
//	snprintf(main_lines[idx++].text,32," %d.%03dv : %3d%% %s",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent,charging);
//	snprintf(main_lines[idx++].text,32,"%d-%02d-%02d %02d:%02d:%02d", clocks->tm_year+1900 , clocks->tm_mon + 1, clocks->tm_mday, clocks->tm_hour, clocks->tm_min, clocks->tm_sec );

	for(int idx=0;idx<16;idx++) { main_lines[idx].length=strlen(main_lines[idx].text); }

	lcd_shader(0,0,240,240,shader_test,0); // interlace updates
/*
	int f=frame&1;
	for(int y=0;y<240;y+=2)
	{
		lcd_shader(0,y+f,240,1,shader_test,0); // interlace updates
	}
*/

	frame++;
	return 0;
}


int main_clock1(int mode)
{
	switch(mode)
	{
		case 1: return main_setup();
		case 2: return main_update();
		case 3: return main_clean();
	}
	return 0;
}

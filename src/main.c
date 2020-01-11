
#include <time.h>
#include <string.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#include "sys/debug.h"

#include "sys/saveram.h"
#include "sys/acc.h"
#include "sys/heart.h"
#include "sys/touch.h"
#include "sys/battery.h"
#include "sys/lcd.h"
#include "sys/clock.h"

// hardware reference manual
// https://infocenter.nordicsemi.com/pdf/nRF52832_OPS_v0.6.3.pdf


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
	int frame=0;
	
	saveram_setup();

	acc_setup();
	heart_setup();
	touch_setup();
	battery_setup();
	lcd_setup();
	clock_setup();

PRINTF("Testing %f!!!\n",1.9f);
	
	// setup text screen buffers for a 30x15 character display.
	for(idx=0;idx<16;idx++)
	{
		lines[idx].name=funfont_8x16r;
		lines[idx].hx=8;
		lines[idx].hy=16;
		lines[idx].foreground=0xffffff;
		lines[idx].dropshadow=0x000000;
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

			time_t t = clock_read();
			struct tm *tm = localtime(&t);
			
			unsigned char * acc=touch_read();

			idx=0;
//			snprintf(lines[idx++].text,32,"Hello World!");
			snprintf(lines[idx++].text,32,"Battery : %d.%03dv : %3d%% %s",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent,charging);
			snprintf(lines[idx++].text,32,"Charge : %3s       Power : %3s", flags&1?"YES":"NO" , flags&2?"YES":"NO" );
			snprintf(lines[idx++].text,32,"%d-%02d-%02d %02d:%02d:%02d", tm->tm_year+1900 , tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec );

			int now = NRF_RTC0->COUNTER ;

			snprintf(lines[idx++].text,32,"Clock Now  %d", now );
			snprintf(lines[idx++].text,32,"Clock Base %d", saveram->clock_base );

/*
			int ai=0;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
			snprintf(lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
*/

			for(idx=0;idx<16;idx++) { lines[idx].length=strlen(lines[idx].text); }

			lcd_shader(0,0,240,240,shader_test,&i);
    
			frame++;
		}

	}

}


#include <time.h>
#include <string.h>

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

static int shader_test(int x,int y,void *data)
{
	int r=-1; // background flag

	int ty=(y/16)&0xf; // 0-15 line
	struct shader_font *line=main_lines+ty;
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


static int main_setup()
{

PRINTF("SETUP TEST\n");

	lcd_color_mode(0x444);
	lcd_rotate(3);

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

static int k=0;
static int i=0;
static int j=0;
static int frame=0;

static int main_update()
{

//PRINTF("UPDATE TEST\n");


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
			case 0: charging="+  +"; break;
			case 1: charging="++  "; break;
			case 2: charging=" ++ "; break;
			case 3: charging="  ++"; break;
		}
	}
	else // Discharging
	{
		switch(frame&3) // animate
		{
			case 0: charging="  --"; break;
			case 1: charging=" -- "; break;
			case 2: charging="--  "; break;
			case 3: charging="-  -"; break;
		}
	}

	time_t t16 = clock_time(); // seconds since 1970 * 65536
	time_t t = t16>>16; // seconds since 1970
	struct tm *tm = localtime(&t);
	
//			unsigned char * acc=touch_read();


	int idx=0;
//			snprintf(lines[idx++].text,32,"Hello World!");
	snprintf(main_lines[idx++].text,32,"Battery : %d.%03dv : %3d%% %s",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent,charging);
	snprintf(main_lines[idx++].text,32,"Charge : %3s       Power : %3s", flags&1?"YES":"NO" , flags&2?"YES":"NO" );
	snprintf(main_lines[idx++].text,32,"%d-%02d-%02d %02d:%02d:%02d", tm->tm_year+1900 , tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec );

	snprintf(main_lines[idx++].text,32,"Clock %u . %04x", (unsigned int)t,(int)(t16&0xffff));

	snprintf(main_lines[idx++].text,32,"Butt %d", main_butt );

/*
	int ai=0;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
	snprintf(main_lines[idx++].text,32,"%02x %02x %02x %02x %02x %02x %02x %02x", acc[ai+0],acc[ai+1],acc[ai+2],acc[ai+3],acc[ai+4],acc[ai+5],acc[ai+6],acc[ai+7] ); ai+=8;
*/

	for(int idx=0;idx<16;idx++) { main_lines[idx].length=strlen(main_lines[idx].text); }

	int f=frame&1;
	for(int y=0;y<240;y+=2)
	{
		lcd_shader(0,y+f,240,1,shader_test,&i); // interlace updates
	}
	frame++;

	return 0;
}


int main_test(int mode)
{
	switch(mode)
	{
		case 1: return main_setup() + main_update() + main_update();
		case 2: return main_update();
		case 3: return main_clean();
	}
	return 0;
}

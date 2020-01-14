
#include <time.h>
#include <string.h>
#include <math.h>

#include <nrf.h>
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
#include "sys/button.h"

#include "main.h"

#include "../art/face.h"
#include "../art/hours.h"
#include "../art/minutes.h"
#include "../art/seconds.h"


static int mtxh[4];
static int mtxm[4];
static int mtxs[4];

static int frame=0;
static struct tm *clocks=0;



static int shader_test(int x,int y,void *data)
{
	int r=-1; // -1 is transparent

	int ty=(y/16)&0xf; // 0-15 line
	struct shader_font *line=main_lines+ty;
	if(line->length>0) // render some text
	{
		r=shader_textline(x,y,line);
	}

	int cx=x-120;
	int cy=y-120;
	
	if(r==-1)
	{
		int rx = ( cx*mtxs[0] + cy*mtxs[1] ) >> 16 ;
		int ry = ( cx*mtxs[2] + cy*mtxs[3] ) >> 16 ;
		r=map_seconds(rx,ry);
	}
	if(r==-1)
	{
		int rx = ( cx*mtxm[0] + cy*mtxm[1] ) >> 16 ;
		int ry = ( cx*mtxm[2] + cy*mtxm[3] ) >> 16 ;
		r=map_minutes(rx,ry);
	}
	if(r==-1)
	{
		int rx = ( cx*mtxh[0] + cy*mtxh[1] ) >> 16 ;
		int ry = ( cx*mtxh[2] + cy*mtxh[3] ) >> 16 ;
		r=map_hours(rx,ry);
	}
	if(r==-1)
	{
		r=map_face(cx,cy);
	}

	if(r==-1) { r=0x000000; }

	return r;
}


static int main_setup()
{
	lcd_color_mode(0x444);
	lcd_rotate(0);

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

static time_t oldt;

static void mtx_rotate(float rad , int mtx[4])
{
	float s=sinf(rad);
	float c=cosf(rad);
	mtx[0] = c*65536;
	mtx[1] = s*65536;
	mtx[2] =-s*65536;
	mtx[3] = c*65536;
}

static int main_update()
{

	time_t t16 = clock_time(); // seconds since 1970 * 65536
	time_t t = t16>>16; // seconds since 1970
	clocks = localtime(&t);

	int newtime=0;
	
	if(oldt!=t) // newtime
	{
		newtime=1;
		oldt=t; // remember
	}
	
	int flags;
	float voltage;
	float percent;
	battery_read(&flags,&voltage,&percent);

	for(int idx=0;idx<16;idx++) { main_lines[idx].text[0]=0; }

	int idx=0;
	if((flags&1)||(percent<20.0f)) // show battery only when charging or when low
	{
		snprintf(main_lines[idx++].text,32,"%d.%03dv                    %3d%%",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent);
	}
	
//	snprintf(main_lines[idx++].text,32,"%d-%02d-%02d %02d:%02d:%02d", clocks->tm_year+1900 , clocks->tm_mon + 1, clocks->tm_mday, clocks->tm_hour, clocks->tm_min, clocks->tm_sec );

	for(int idx=0;idx<16;idx++) { main_lines[idx].length=strlen(main_lines[idx].text); }


	// only draw if time changes
	if(newtime)
	{
		int s=clocks->tm_sec;
		int m=clocks->tm_min;
		int h=clocks->tm_hour%12;
		
		mtx_rotate( s           *((float)(2.0*M_PI/60.0)),mtxs);
		mtx_rotate((m+(s/60.0f))*((float)(2.0*M_PI/60.0)),mtxm);
		mtx_rotate((h+(m/60.0f))*((float)(2.0*M_PI/60.0)),mtxh);
		lcd_shader(0,0,240,240,shader_test,0);
	}
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
		case 1:
			main_setup();
			main_update();
			nrf_delay_ms(1000/10); // need to catch next frame
			lcd_backlight(0x60); // display on
			nrf_delay_ms(1000/10);
			lcd_backlight(0xa0); // fade
			nrf_delay_ms(1000/10);
			lcd_backlight(0xff); // fade
		break;
		case 2:
			main_update();
		break;
		case 3:
			lcd_backlight(0xa0); // fade
			nrf_delay_ms(1000/10);
			lcd_backlight(0x60); // fade
			nrf_delay_ms(1000/10);
			lcd_backlight(0); // display off
			main_clean();
		break;
	}
	return 0;
}

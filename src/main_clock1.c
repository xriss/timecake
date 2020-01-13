
#include <time.h>
#include <string.h>
#include <math.h>

#include <nrf.h>
#include "nrf_gpio.h"

#include "fixmath.h"


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
//		int dd=d*d;
		int cx=x-120.0f;
		int cy=y-120.0f;
		int cxa=cx < 0 ? -cx : cx; // abs
		int cya=cy < 0 ? -cy : cy; // abs
		
		int dr=0; // distance around perimiter
		int dc=0; // distance to center
		
		if( (cxa==0) && (cya==0) ) // deal with 0 0 case
		{
			dr=0;
			dc=0;
		}
		else
		if(cxa<cya)
		{
			dc=(cya<<16)/d;
			if(cy<0)
			{
				dr=0x0000+((cx<<13)/cya);
			}
			else
			{
				dr=0x8000-((cx<<13)/cya);
			}
		}
		else
		{
			dc=(cxa<<16)/d;
			if(cx<0)
			{
				dr=0xc000-((cy<<13)/cxa);
			}
			else
			{
				dr=0x4000+((cy<<13)/cxa);
			}
		}
		dr=dr&0xffff;
		
		r=0x000000;
		int p; // polarity

//		int s; // width of hand

/*
		if(dc<0x8000)
		{
			p=1;
			if( dr*365 < clocks->tm_yday<<16 ) { p=p*-1; }
			if(p>0) { r+=0x111111;} else { r-=0x111111; }
		}
//		else
		if(dc<0xa000)
		{
			if(clocks->tm_yday&1) { p=1; } else { p=-1; }
			if( dr*31 < clocks->tm_mday<<16 ) { p=p*-1; }
			if(p>0) { r+=0x111111;} else { r-=0x111111; }
		}
//		else
*/
		if(dc<0x8000)
		{
/*
			if(clocks->tm_hour&1) { p=1; } else { p=-1; }
			if( dr*12 < (clocks->tm_hour%12)<<16 ) { p=p*-1; }
			if(p>0) { r+=0x333333;} // else { r-=0x111111; }
*/
			int s=(0x0000+0xffff-dc);
			s=s>>8;
			s=(s*s)>>6;
			p=( dr - ( ((clocks->tm_hour%12)<<16) / 12 ) - ( (clocks->tm_min<<16) / (60*60) ) )&0xffff;
			if(p>0x8000) { p=0xffff-p; }
			if(p<=s) { r=0xffffff; }
			else
			if(p<=s*2) { p=p-s; p=(p<<8)/s; r=p+(p<<8)+(p<<16); }
//			p=(0xffff-p)>>10;
//			r+=((p<<16)&0xff0000)+((p<<8)&0xff00)+(p&0xff);
		}
//		else
		if(dc<0xc000)
		{
/*
			if(clocks->tm_hour&1) { p=1; } else { p=-1; }
			if( dr*60 < clocks->tm_min<<16 ) { p=p*-1; }
			if(p>0) { r+=0x333333;} // else { r-=0x222222; }
*/
			int s=(0x0000+0xffff-dc);
			s=s>>8;
			s=(s*s)>>6;
			p=(dr - ( (clocks->tm_min<<16) / 60 ) - ( (clocks->tm_sec<<16) / (60*60) ) )&0xffff;
			if(p>0x8000) { p=0xffff-p; }
			if(p<=s) { r=0xffffff; }
			else
			if(p<=s*2) { p=p-s; p=(p<<8)/s; r=p+(p<<8)+(p<<16); }
//			p=(0xffff-p)>>10;
//			r+=((p<<16)&0xff0000)+((p<<8)&0xff00)+(p&0xff);
		}
//		else
		{
/*
			if(clocks->tm_min&1) { p=1; } else { p=-1; }
			if( dr*60 < clocks->tm_sec<<16 ) { p=p*-1; }
			if(p>0) { r+=0x333333;} // else { r-=0x333333; }
*/
			int s=(0x0000+0xffff-dc);
			s=s>>8;
			s=(s*s)>>6;
			p=(dr - ( (clocks->tm_sec<<16) / 60 ) )&0xffff;
			if(p>0x8000) { p=0xffff-p; }
			if(p<=s) { r=0xffffff; }
			else
			if(p<=s*2) { p=p-s; p=(p<<8)/s; r=p+(p<<8)+(p<<16); }
//			p=(0xffff-p)>>10;
//			r+=((p<<16)&0xff0000)+((p<<8)&0xff00)+(p&0xff);
		}

	}
	return r;
}


static int main_setup()
{
	lcd_color_mode(0x444);

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

	int idx=0;

	if(flags&1)
	{
		snprintf(main_lines[idx++].text,32," %d.%03dv : %3d%% %s",(int)voltage,(int)((voltage-(int)voltage)*1000.0f),(int)percent,charging);
	}
	
//	snprintf(main_lines[idx++].text,32,"%d-%02d-%02d %02d:%02d:%02d", clocks->tm_year+1900 , clocks->tm_mon + 1, clocks->tm_mday, clocks->tm_hour, clocks->tm_min, clocks->tm_sec );

	for(int idx=0;idx<16;idx++) { main_lines[idx].length=strlen(main_lines[idx].text); }

	// only draw if time changes
	if(newtime)
	{
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
		case 1: return main_setup();
		case 2: return main_update();
		case 3: return main_clean();
	}
	return 0;
}

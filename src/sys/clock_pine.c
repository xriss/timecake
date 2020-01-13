
// pine clock code


#include <time.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "debug.h"
#include "clock.h"
#include "saveram.h"

#define CLOCK_POW			15					// must be between 3 and 15
#define CLOCK_SCALE			(1<<CLOCK_POW)		// number of ticks per second 8 to 32768


// something like this
// https://github.com/NordicPlayground/nrf5-calendar-example/blob/master/nrf_calendar.c

static int tickoff=0;

/*
	initialize hardware counters
*/
int clock_setup(void)
{
	time_t t = time(NULL); // this will return -1 or the correct time if semihosting is working

	if(t>TIME_OF_WRITING) // if time is after 2020 then assume it is correct
	{
		saveram->clock=t*65536;
	}

	// Select the 32 kHz crystal and start the 32 kHz clock
	NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos;
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);

	NRF_RTC0->PRESCALER = (0xfff>>(CLOCK_POW-3));
	NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
	NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
	NRF_RTC0->CC[0] = CLOCK_SCALE ; // 1 second interrupt
	NVIC_SetPriority(RTC0_IRQn, 3);
	NVIC_EnableIRQ(RTC0_IRQn);
	NRF_RTC0->TASKS_START = 1;
    NRF_RTC0->TASKS_CLEAR = 1; // start
    tickoff=0;
    
	return 0;
}

void RTC0_IRQHandler(void)
{
	if(NRF_RTC0->EVENTS_COMPARE[0])
	{
		NRF_RTC0->EVENTS_COMPARE[0] = 0;
		saveram->clock+=65536; // one second has passed
		tickoff=NRF_RTC0->COUNTER&0xff8000; // 24bit counter
		NRF_RTC0->CC[0] = ((tickoff+CLOCK_SCALE)&0xff8000) ; // next 1 second interrupt
	}
}

/*
	get linux time in seconds<<16
	
*/
long long int clock_time(void)
{
	volatile int t;
	volatile long long int r;
	
	NVIC_DisableIRQ(RTC0_IRQn); // not sure how to guard this access...
	t=NRF_RTC0->COUNTER;
	r=saveram->clock;
	NVIC_EnableIRQ(RTC0_IRQn);
	
	r+= ( ((t-tickoff)&0xffffff) << (16-CLOCK_POW) ) ;
	
//	PRINTF("ticks %08x\n",(t-tickoff) << (16-CLOCK_POW));
	
	return r;
}

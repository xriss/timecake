
// pine clock code


#include <time.h>

#include "nrf.h"

#include "clock.h"
#include "saveram.h"



// something like this
// https://github.com/NordicPlayground/nrf5-calendar-example/blob/master/nrf_calendar.c


/*
	initialize hardware counters
*/
int clock_setup(void)
{
	time_t t = time(NULL); // this will return -1 or the correct time if semihosting is working

	if(t>TIME_OF_WRITING) // if time is after 2020 then assume it is correct
	{
		saveram->clock_base=t;
	}

	// Select the 32 kHz crystal and start the 32 kHz clock
	NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos;
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);

	// Configure the RTC for 1 minute wakeup (default)
	NRF_RTC0->PRESCALER = 0xFFF; // 1 second tick
	NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
	NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
	NRF_RTC0->CC[0] = 60*8 ; // 60 seconds interrupt
	NRF_RTC0->TASKS_START = 1;
	NVIC_SetPriority(RTC0_IRQn, 3);
	NVIC_EnableIRQ(RTC0_IRQn);
    NRF_RTC0->TASKS_CLEAR = 1; // enable next interupt
    
	return 0;
}

void RTC0_IRQHandler(void)
{
	if(NRF_RTC0->EVENTS_COMPARE[0])
	{
		NRF_RTC0->EVENTS_COMPARE[0] = 0;
		NRF_RTC0->TASKS_CLEAR = 1; // enable next interupt
		saveram->clock_base+=60; // one minute has passed
	}
}

/*
	get linux time in seconds
*/
unsigned int clock_read(void)
{
	int now = NRF_RTC0->COUNTER ;
	return saveram->clock_base + (now/8) ; //  current time
}

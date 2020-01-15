
// pine battery code

// https://github.com/andenore/NordicSnippets/blob/master/examples/saadc


#include "battery.h"

#include <nrf.h>
#include "nrf_gpio.h"


#define BATTERY_CHARGE  12
#define BATTERY_POWER   19
#define BATTERY_HALF_VOLTAGE SAADC_CH_PSELP_PSELP_AnalogInput7

/*

setup all battery related stuff

*/
int battery_setup(void)
{
	nrf_gpio_cfg_input(BATTERY_CHARGE, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup);
	nrf_gpio_cfg_input(BATTERY_POWER, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup);

	// Start HFCLK from crystal oscillator, this will give the SAADC higher accuracy
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;

	// Configure SAADC singled-ended channel, Internal reference (0.6V) and 1/6 gain.
	NRF_SAADC->CH[0].CONFIG =	(SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos) |
								(SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos) |
								(SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) |
								(SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos) |
								(SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos) |
								(SAADC_CH_CONFIG_TACQ_3us        << SAADC_CH_CONFIG_TACQ_Pos);

	// Configure the SAADC channel with VDD as positive input, no negative input(single ended).
//	NRF_SAADC->CH[0].PSELP = SAADC_CH_PSELP_PSELP_VDD << SAADC_CH_PSELP_PSELP_Pos;
	NRF_SAADC->CH[0].PSELP = BATTERY_HALF_VOLTAGE << SAADC_CH_PSELP_PSELP_Pos;
	NRF_SAADC->CH[0].PSELN = SAADC_CH_PSELN_PSELN_NC << SAADC_CH_PSELN_PSELN_Pos;

	// Configure the SAADC resolution.
	NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_14bit << SAADC_RESOLUTION_VAL_Pos;

	// No automatic sampling, will trigger with TASKS_SAMPLE.
	NRF_SAADC->SAMPLERATE = SAADC_SAMPLERATE_MODE_Task << SAADC_SAMPLERATE_MODE_Pos;

	// Enable SAADC (would capture analog pins if they were used in CH[0].PSELP)
	NRF_SAADC->ENABLE = SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos;



// Calibrate the SAADC (only needs to be done once in a while)
	NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
	while (NRF_SAADC->EVENTS_CALIBRATEDONE == 0);
	NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
	while (NRF_SAADC->STATUS == (SAADC_STATUS_STATUS_Busy <<SAADC_STATUS_STATUS_Pos));

	return 0;
}



/*

read the current settings from the battery

	(flags & 1) == CHARGE
	(flags & 2) == POWER

I assumed charge false and power true , would mean we have finished 
charging. Please be aware that we never seem to reach that state so 
best to ignore the power flag and just look as the charge flag.



*/
void battery_read(int *flags,float *voltage,float *percent)
{
	*flags=0;
	if( !nrf_gpio_pin_read(BATTERY_CHARGE) ) { *flags|=1; }
	if( !nrf_gpio_pin_read(BATTERY_POWER) )  { *flags|=2; }

	volatile int16_t result = 0;
	volatile float precise_result = 0;

	// Configure result to be put in RAM at the location of "result" variable.
	NRF_SAADC->RESULT.MAXCNT = 1;
	NRF_SAADC->RESULT.PTR = (uint32_t)&result;

	// Start the SAADC and wait for the started event.
	NRF_SAADC->TASKS_START = 1;
	while (NRF_SAADC->EVENTS_STARTED == 0);
	NRF_SAADC->EVENTS_STARTED = 0;

	// Do a SAADC sample, will put the result in the configured RAM buffer.
	NRF_SAADC->TASKS_SAMPLE = 1;
	while (NRF_SAADC->EVENTS_END == 0);
	NRF_SAADC->EVENTS_END = 0;

	// Convert the result to voltage
	// Result = [V(p) - V(n)] * GAIN/REFERENCE * 2^(RESOLUTION)
	// Result = (VDD - 0) * ((1/6) / 0.6) * 2^14
	// VDD = Result / 4551.1
//	precise_result = (float)result / 4551.1f;
	precise_result = (float)result / (4551.1f/2.0f);

	// Stop the SAADC, since it's not used anymore.
	NRF_SAADC->TASKS_STOP = 1;
	while (NRF_SAADC->EVENTS_STOPPED == 0);
	NRF_SAADC->EVENTS_STOPPED = 0;
  
	*voltage = precise_result;

	// see https://forum.pine64.org/showthread.php?tid=8147
	// voltage falls from around 4.1v to 2.9v ish with a sharp fall off after 3.5v
	*percent = ((precise_result - 2.9f)*100.0f)/(4.1f-2.9f);
	if(*percent<=50.0f) // 0% to 10% : 2.9v to 3.5v
	{
		*percent=*percent/5.0f; 
	}
	else // 10% to 100% : 3.5v to 4.1v
	{
		*percent=10.0f+((*percent-50.0f)*(90.0f/50.0f));
	}
	// sanity clamp
	if(*percent<0.0f) { *percent=0.0f; }
	if(*percent>100.0f) { *percent=100.0f; }
}

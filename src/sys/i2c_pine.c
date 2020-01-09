
// pine i2c code

// https://github.com/andenore/NordicSnippets/tree/master/examples/i2c_master


#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"


#define PIN_SCL        (7)
#define PIN_SDA        (6)


int i2c_setup(void)
{
	NRF_TWIM1->PSEL.SCL = PIN_SCL;
	NRF_TWIM1->PSEL.SDA = PIN_SDA;
	
	NRF_TWIM1->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400 << TWIM_FREQUENCY_FREQUENCY_Pos;
	NRF_TWIM1->SHORTS = 0;
	
	NRF_TWIM1->ENABLE = TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos;

	return 0;
}


void i2c_write(int device,int length,uint8_t *data)
{
	NRF_TWIM1->SHORTS = TWIM_SHORTS_LASTTX_STOP_Msk;

	NRF_TWIM1->ADDRESS = device;

	NRF_TWIM1->TXD.MAXCNT = length;
	NRF_TWIM1->TXD.PTR = (uint32_t)data;

	NRF_TWIM1->EVENTS_STOPPED = 0;
	NRF_TWIM1->TASKS_STARTTX = 1;
	while((NRF_TWIM1->EVENTS_STOPPED == 0)&&(NRF_TWIM1->EVENTS_ERROR == 0)){}

}

void i2c_read(int device,int address,int length,uint8_t *data)
{
	uint8_t tx_buf[1];
	NRF_TWIM1->SHORTS = TWIM_SHORTS_LASTTX_STARTRX_Msk | TWIM_SHORTS_LASTRX_STOP_Msk;

	NRF_TWIM1->ADDRESS = device;

	tx_buf[0] = address;
	NRF_TWIM1->TXD.MAXCNT = 1;
	NRF_TWIM1->TXD.PTR = (uint32_t)tx_buf;

	NRF_TWIM1->RXD.MAXCNT = length;
	NRF_TWIM1->RXD.PTR = (uint32_t)data;

	NRF_TWIM1->EVENTS_STOPPED = 0;
	NRF_TWIM1->TASKS_STARTTX = 1;
	while((NRF_TWIM1->EVENTS_STOPPED == 0)&&(NRF_TWIM1->EVENTS_ERROR == 0)){}

}



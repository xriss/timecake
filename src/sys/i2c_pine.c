
// pine i2c code

// https://github.com/andenore/NordicSnippets/tree/master/examples/i2c_master


#include <nrf.h>


#define PIN_SCL        (7)
#define PIN_SDA        (6)


int i2c_setup(void)
{
	NRF_TWIM0->PSEL.SCL = PIN_SCL;
	NRF_TWIM0->PSEL.SDA = PIN_SDA;

	NRF_TWIM0->ADDRESS = 0x18;

	NRF_TWIM0->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400 << TWIM_FREQUENCY_FREQUENCY_Pos;
	NRF_TWIM0->SHORTS = 0;

	NRF_TWIM0->ENABLE = TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos;
	
	return 0;
}


void i2c_write(int device,int length,uint8_t *data)
{
	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STOP_Msk;

	NRF_TWIM0->ADDRESS = device;

	NRF_TWIM0->TXD.MAXCNT = length;
	NRF_TWIM0->TXD.PTR = (uint32_t)data;

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);
}

void i2c_read(int device,int length,int address,uint8_t *data)
{
	uint8_t tx_buf[1];
	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STARTRX_Msk | TWIM_SHORTS_LASTRX_STOP_Msk;

	NRF_TWIM0->ADDRESS = device;

	tx_buf[0] = address;
	NRF_TWIM0->TXD.MAXCNT = sizeof(tx_buf);
	NRF_TWIM0->TXD.PTR = (uint32_t)&tx_buf[0];

	NRF_TWIM0->RXD.MAXCNT = length;
	NRF_TWIM0->RXD.PTR = (uint32_t)data;

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);

}



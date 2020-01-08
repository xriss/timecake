
// pine i2c code

// https://github.com/andenore/NordicSnippets/tree/master/examples/i2c_master


#include <nrf.h>


#define PIN_SCL        (7)
#define PIN_SDA        (6)



void i2c_write(uint8_t addr, uint8_t data)
{
	uint8_t tx_buf[2];
	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STOP_Msk;

	tx_buf[0] = addr;
	tx_buf[1] = data;
	NRF_TWIM0->TXD.MAXCNT = sizeof(tx_buf);
	NRF_TWIM0->TXD.PTR = (uint32_t)&tx_buf[0];

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);
}

uint8_t i2c_read(uint8_t addr)
{
	uint8_t tx_buf[1];
	uint8_t rx_buf[1];
	NRF_TWIM0->SHORTS = TWIM_SHORTS_LASTTX_STARTRX_Msk | TWIM_SHORTS_LASTRX_STOP_Msk;

	tx_buf[0] = addr;
	NRF_TWIM0->TXD.MAXCNT = sizeof(tx_buf);
	NRF_TWIM0->TXD.PTR = (uint32_t)&tx_buf[0];

	NRF_TWIM0->RXD.MAXCNT = 1;
	NRF_TWIM0->RXD.PTR = (uint32_t)&rx_buf[0];

	NRF_TWIM0->EVENTS_STOPPED = 0;
	NRF_TWIM0->TASKS_STARTTX = 1;
	while (NRF_TWIM0->EVENTS_STOPPED == 0);

	return rx_buf[0];
}


int i2c_setup(void)
{
	volatile uint8_t data;

	NRF_TWIM0->PSEL.SCL = PIN_SCL;
	NRF_TWIM0->PSEL.SDA = PIN_SDA;

	NRF_TWIM0->ADDRESS = DEVICE_ADDRESS;
	NRF_TWIM0->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400 << TWIM_FREQUENCY_FREQUENCY_Pos;
	NRF_TWIM0->SHORTS = 0;

	NRF_TWIM0->ENABLE = TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos;

	heart_write(0x0, 0xFF);

	data = heart_read(0x0);
	data; // to get rid of set but unused warning

	while (1)
	{
		__WFE();
	}
}





// pine acc code

#include <nrf.h>

#include "i2c_pine.h"

#define ACC_I2C_DEVICE 0x18


static unsigned char acc_data[128];


int acc_setup(void)
{
	i2c_setup(); // this will may called multiple times
	
	return 0;
}

unsigned char * acc_read(void)
{
	i2c_read(ACC_I2C_DEVICE,128,0,acc_data);
	
	return acc_data;
}


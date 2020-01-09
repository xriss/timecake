
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
/*
	acc_data[0]=0x2D;
	acc_data[1]=0x08;
	i2c_write(ACC_I2C_DEVICE,1,acc_data);
*/
	
	i2c_read(ACC_I2C_DEVICE,0,128,acc_data);
	
	return acc_data;
}


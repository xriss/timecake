
// pine heart code

#include <nrf.h>

#include "i2c_pine.h"

#define HEART_I2C_DEVICE (0x44)

static unsigned char heart_data[128];


int heart_setup(void)
{
	i2c_setup(); // this will may called multiple times
	
	return 0;
}


unsigned char * heart_read(void)
{
/*
	acc_data[0]=0x2D;
	acc_data[1]=0x08;
	i2c_write(ACC_I2C_DEVICE,1,acc_data);
*/
	
	i2c_read(HEART_I2C_DEVICE,0,128,heart_data);
	
	return heart_data;
}

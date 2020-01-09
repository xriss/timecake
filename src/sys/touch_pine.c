
// pine touch code

#include <nrf.h>

#include "i2c_pine.h"

#define TOUCH_I2C_DEVICE (0x15)


static unsigned char touch_data[128];


int touch_setup(void)
{
	i2c_setup(); // this will may called multiple times

	return 0;
}

unsigned char * touch_read(void)
{
/*
	acc_data[0]=0x2D;
	acc_data[1]=0x08;
	i2c_write(ACC_I2C_DEVICE,1,acc_data);
*/
	
	i2c_read(TOUCH_I2C_DEVICE,0,128,touch_data);
	
	return touch_data;
}


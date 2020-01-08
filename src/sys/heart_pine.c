
// pine heart code

#include <nrf.h>

#include "i2c_pine.h"

#define HEART_I2C_ADDRESS (0x44)

int heart_setup(void)
{
	i2c_setup(); // this will may called multiple times
	
	return 0;
}

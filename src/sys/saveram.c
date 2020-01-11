
// generic saveram code

#include "debug.h"
#include "saveram.h"
#include "string.h"
#include "clock.h"



int saveram_setup(void)
{
	if( ( saveram->magick  != SAVERAM_MAGICK   ) ||
	    ( saveram->version != SAVERAM_VERSION  ) ||
	    ( saveram->length  != sizeof(saveram)  ) ) // SIMPLE VALIDATION CHECK
	{
		saveram_format();
	}
	return 0;
}

void saveram_format(void)
{
	memset(saveram,0xf0,sizeof(saveram)); // check we can write to all of it
	for(int i=0;i<sizeof(saveram);i++)
	{
		if( ((char*)saveram)[i]!=0xf0 )
		{
			PRINTF("Saveram write FAIL!");
			while(1);
		}
	}
	memset(saveram,0x00,sizeof(saveram));
	
	saveram->magick  = SAVERAM_MAGICK;
	saveram->version = SAVERAM_VERSION;
	saveram->length  = sizeof(saveram);

	saveram->clock_base=TIME_OF_WRITING; // it will be at least 2020

}


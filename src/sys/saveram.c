
// generic saveram code

#include "saveram.h"

#include "string.h"


int saveram_setup(void)
{
	if( ( saveram->magick  != SAVERAM_MAGICK   ) ||
	    ( saveram->version != SAVERAM_VERSION  ) ||
	    ( saveram->length  != sizeof(saveram)  ) ) // SIMPLE VALIDATION CHECK
	{
		saveram_format();
	}
	return 1;
}

void saveram_format(void)
{
	memset(saveram,0,sizeof(saveram));
	
	saveram->magick  = SAVERAM_MAGICK;
	saveram->version = SAVERAM_VERSION;
	saveram->length  = sizeof(saveram);
}


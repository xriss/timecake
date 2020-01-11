

extern int saveram_setup(void);
extern void saveram_format(void);


#define SAVERAM_MAGICK  0x5AB3BEEF
#define SAVERAM_VERSION 0x00010002

struct saveram 
{
	unsigned int magick;   // magick number 
	unsigned int version;  // version
	unsigned int length;   // size of this saveram struct
	
	unsigned int clock_base; // add this to the hardware clock sample to get time
};


extern struct saveram saveram[1];

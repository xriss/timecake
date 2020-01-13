

extern int saveram_setup(void);
extern void saveram_format(void);


#define SAVERAM_MAGICK  0x5AB3BEEF
#define SAVERAM_VERSION 0x00010008

struct saveram 
{
	unsigned int magick;   // magick number 
	unsigned int version;  // version
	unsigned int length;   // size of this saveram struct
	
	long long int clock;   // time in seconds, live updated by interrupt
};


extern struct saveram saveram[1];

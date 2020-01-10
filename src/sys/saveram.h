

extern int saveram_setup(void);
extern void saveram_format(void);


#define SAVERAM_MAGICK  0x5AB3BEEF
#define SAVERAM_VERSION 0x00010001

struct saveram 
{
	unsigned int magick;   // magick number 
	unsigned int version;  // version
	unsigned int length;   // size of this saveram struct
	
	int test1;
	int test2;
	int test3;
};


extern struct saveram saveram[1];

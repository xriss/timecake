
// pine lcd code

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "lcd.h"
  
#define LCD_SCK             2
#define LCD_MOSI            3
#define LCD_MISO            4
#define LCD_SELECT         25
#define LCD_COMMAND        18
#define LCD_RESET          26

#define LCD_BACKLIGHT_LOW  14
#define LCD_BACKLIGHT_MID  22
#define LCD_BACKLIGHT_HIGH 23

// sitronix ST7789V LCD commands from https://wiki.pine64.org/images/5/54/ST7789V_v1.6.pdf

#define CMD_NOP       0x00 // NOP
#define CMD_SWRESET   0x01 // Software Reset
#define CMD_RDDID     0x04 // Read Display ID
#define CMD_RDDST     0x09 // Read Display Status
#define CMD_RDDPM     0x0A // Read Display Power Mode
#define CMD_RDDMADCTL 0x0B // Read Display MADCTL
#define CMD_RDDCOLMOD 0x0C // Read Display Pixel Format
#define CMD_RDDIM     0x0D // Read Display Image Mode
#define CMD_RDDSM     0x0E // Read Display Signal Mode
#define CMD_RDDSDR    0x0F // Read Display Self-Diagnostic Result
#define CMD_SLPIN     0x10 // Sleep in
#define CMD_SLPOUT    0x11 // Sleep Out
#define CMD_PTLON     0x12 // Partial Display Mode On
#define CMD_NORON     0x13 // Normal Display Mode On
#define CMD_INVOFF    0x20 // Display Inversion Off
#define CMD_INVON     0x21 // Display Inversion On
#define CMD_GAMSET    0x26 // Gamma Set
#define CMD_DISPOFF   0x28 // Display Off
#define CMD_DISPON    0x29 // Display On
#define CMD_CASET     0x2A // Column Address Set
#define CMD_RASET     0x2B // Row Address Set
#define CMD_RAMWR     0x2C // Memory Write
#define CMD_RAMRD     0x2E // Memory Read
#define CMD_PTLAR     0x30 // Partial Area
#define CMD_VSCRDEF   0x33 // Vertical Scrolling Definition
#define CMD_TEOFF     0x34 // Tearing Effect Line OFF
#define CMD_TEON      0x35 // Tearing Effect Line On
#define CMD_MADCTL    0x36 // Memory Data Access Control
#define CMD_VSCSAD    0x37 // Vertical Scroll Start Address of RAM
#define CMD_IDMOFF    0x38 // Idle Mode Off
#define CMD_IDMON     0x39 // Idle mode on
#define CMD_COLMOD    0x3A // Interface Pixel Format
#define CMD_WRMEMC    0x3C // Write Memory Continue
#define CMD_RDMEMC    0x3E // Read Memory Continue
#define CMD_STE       0x44 // Set Tear Scanline
#define CMD_GSCAN     0x45 // Get Scanline
#define CMD_WRDISBV   0x51 // Write Display Brightness
#define CMD_RDDISBV   0x52 // Read Display Brightness Value
#define CMD_WRCTRLD   0x53 // Write CTRL Display
#define CMD_RDCTRLD   0x54 // Read CTRL Value Display
#define CMD_WRCACE    0x55 // Write Content Adaptive Brightness Control and Color Enhancement
#define CMD_RDCABC    0x56 // Read Content Adaptive Brightness Control
#define CMD_WRCABCMB  0x5E // Write CABC Minimum Brightness
#define CMD_RDCABCMB  0x5F // Read CABC Minimum Brightness
#define CMD_RDABCSDR  0x68 // Read Automatic Brightness Control Self-Diagnostic Result
#define CMD_RDID1     0xDA // Read ID1
#define CMD_RDID2     0xDB // Read ID2
#define CMD_RDID3     0xDC // Read ID3
#define CMD_RAMCTRL   0xB0 // RAM Control
#define CMD_RGBCTRL   0xB1 // RGB Interface Control
#define CMD_PORCTRL   0xB2 // Porch Setting
#define CMD_FRCTRL1   0xB3 // Frame Rate Control 1 0xIn partial mode/ idle colors)
#define CMD_PARCTRL   0xB5 // Partial Control
#define CMD_GCTRL     0xB7 // Gate Control
#define CMD_GTADJ     0xB8 // Gate On Timing Adjustment
#define CMD_DGMEN     0xBA // Digital Gamma Enable
#define CMD_VCOMS     0xBB // VCOM Setting
#define CMD_LCMCTRL   0xC0 // LCM Control
#define CMD_IDSET     0xC1 // ID Code Setting
#define CMD_VDVVRHEN  0xC2 // VDV and VRH Command Enable
#define CMD_VRHS      0xC3 // VRH Set
#define CMD_VDVS      0xC4 // VDV Set
#define CMD_VCMOFSET  0xC5 // VCOM Offset Set
#define CMD_FRCTRL2   0xC6 // Frame Rate Control in Normal Mode
#define CMD_CABCCTRL  0xC7 // CABC Control
#define CMD_REGSEL1   0xC8 // Register Value Selection 1
#define CMD_REGSEL2   0xCA // Register Value Selection 2
#define CMD_PWMFRSEL  0xCC // PWM Frequency Selection
#define CMD_PWCTRL1   0xD0 // Power Control 1
#define CMD_VAPVANEN  0xD2 // Enable VAP/VAN signal output
#define CMD_CMD2EN    0xDF // Command 2 Enable
#define CMD_PVGAMCTRL 0xE0 // Positive Voltage Gamma Control
#define CMD_NVGAMCTRL 0xE1 // Negative Voltage Gamma Control
#define CMD_DGMLUTR   0xE2 // Digital Gamma Look-up Table for Red
#define CMD_DGMLUTB   0xE3 // Digital Gamma Look-up Table for Blue
#define CMD_GATECTRL  0xE4 // Gate Control
#define CMD_SPI2EN    0xE7 // SPI2 Enable
#define CMD_PWCTRL2   0xE8 // Power Control 2
#define CMD_EQCTRL    0xE9 // Equalize time control
#define CMD_PROMCTRL  0xEC // Program Mode Control
#define CMD_PROMEN    0xFA // Program Mode Enable
#define CMD_NVMSET    0xFC // NVM Setting
#define CMD_PROMACT   0xFE // Program action 


// send and receive data
static inline int lcd_transfer(int t)
{
	volatile uint32_t r;
	NRF_SPI0->EVENTS_READY=0;           // ready
	NRF_SPI0->TXD=(uint32_t)t;          // out
    while(NRF_SPI0->EVENTS_READY==0){;} // wait
	r=NRF_SPI0->RXD;                    // in
	return (int)r;
}

// send a command byte
static inline int lcd_command(int t)
{
	nrf_gpio_pin_write(LCD_COMMAND,0);
	return lcd_transfer(t);
}

// send a data byte
static inline int lcd_data(int t)
{
	nrf_gpio_pin_write(LCD_COMMAND,1);
	return lcd_transfer(t);
}

// send a data word
static inline void lcd_data_word(int t)
{
	nrf_gpio_pin_write(LCD_COMMAND,1);
	lcd_transfer(t>>8);
	lcd_transfer(t&0xff);
}

// select rectangular area to read from or write to
static inline void lcd_window(int px,int py,int hx,int hy)
{
	lcd_command(CMD_CASET);
	lcd_data_word(px);
	lcd_data_word((px+hx-1));

	lcd_command(CMD_RASET);
	lcd_data_word(py);
	lcd_data_word((py+hy-1));
}

/*

setup all LCD related stuff

*/
int lcd_setup(void)
{

	nrf_gpio_cfg_output(LCD_BACKLIGHT_LOW);
	nrf_gpio_cfg_output(LCD_BACKLIGHT_MID);
	nrf_gpio_cfg_output(LCD_BACKLIGHT_HIGH);	

	nrf_gpio_cfg_output(LCD_SCK);
	nrf_gpio_cfg_output(LCD_MOSI);
	nrf_gpio_cfg_input(LCD_MISO, NRF_GPIO_PIN_NOPULL);

	nrf_gpio_cfg_output(LCD_SELECT);
	nrf_gpio_cfg_output(LCD_COMMAND);
	nrf_gpio_cfg_output(LCD_RESET);

	nrf_gpio_pin_write(LCD_SELECT,0); // select LCD
	nrf_gpio_pin_write(LCD_COMMAND,1);
	nrf_gpio_pin_write(LCD_RESET,1);

	NRF_SPI0->ENABLE=0;

	NRF_SPI0->PSELSCK=LCD_SCK;
	NRF_SPI0->PSELMOSI=LCD_MOSI;
	NRF_SPI0->PSELMISO=LCD_MISO;
	NRF_SPI0->FREQUENCY=SPI_FREQUENCY_FREQUENCY_M8;

	NRF_SPI0->CONFIG=(0x03 << 1);
	NRF_SPI0->EVENTS_READY=0;
	NRF_SPI0->ENABLE=(SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);

	// hardware reset
	nrf_gpio_pin_write(LCD_RESET,0);
	nrf_delay_ms(200);
	nrf_gpio_pin_write(LCD_RESET,1);

	// software reset
	lcd_command(CMD_SWRESET); nrf_delay_ms(200);

	// sleep off
	lcd_command(CMD_SLPOUT);
			
	// select 444 pixel format (Which is fastest to write)
	lcd_color_mode(0x444);
	lcd_rotate(0);

	// set screen to black
	int c=0x000000;
	lcd_shader(0,0,240,240,lcd_shader_color,&c);

	lcd_command(CMD_INVON);
	lcd_command(CMD_NORON);

	lcd_backlight(0xff);
	
	
	return 0;
}


/*

Rotate the display address mode.

0 == normal
1 == rotate clockwise 90
2 == rotate clockwise 180
3 == rotate clockwise 270

*/
int lcd_rotate(int rot)
{
	switch(rot)
	{
		case 0:
			lcd_command(CMD_MADCTL); lcd_data(0x00);
			lcd_command(CMD_VSCSAD); lcd_data_word(0);
		break;
		case 1:
			lcd_command(CMD_MADCTL); lcd_data(0x60);
			lcd_command(CMD_VSCSAD); lcd_data_word(0);
		break;
		case 2:
			lcd_command(CMD_MADCTL); lcd_data(0xc0);
			lcd_command(CMD_VSCSAD); lcd_data_word(80);
		break;
		case 3:
			lcd_command(CMD_MADCTL); lcd_data(0xa0);
			lcd_command(CMD_VSCSAD); lcd_data_word(80);
		break;
		default:
			return 1; // error
		break;
	}
	return 0;
}

/*

set sleep mode.

0   awake
255 asleep

*/
int lcd_sleep(int sleep)
{
	if(sleep)
	{
		lcd_command(CMD_SLPIN);
	}
	else
	{
		lcd_command(CMD_SLPOUT);
	}
	return 0;
}

/*

Only 4 levels but use a bright value from 0 to 255

0x00 == off
0x40 == low
0x80 == medium
0xc0 == high

*/
int lcd_backlight(int bright)
{


	if(bright<0x40) // darkest
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_MID,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,1);
	}
	else
	if(bright<0x80)
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,0);
		nrf_gpio_pin_write(LCD_BACKLIGHT_MID,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,1);
	}
	else
	if(bright<0xc0)
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_MID,0);
		nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,1);
	}
	else
	{
		nrf_gpio_pin_write(LCD_BACKLIGHT_LOW,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_MID,1);
		nrf_gpio_pin_write(LCD_BACKLIGHT_HIGH,0);
	}

	// display on/off
	if(bright==0)
	{
		lcd_command(CMD_DISPOFF);
	}
	else
	{
		lcd_command(CMD_DISPON);
	}

	return 0;
}

int lcd_shader_color(int x,int y,void *data)
{
	int d=*((int*)data);
	return d;
}

// shade a rectangular area with a function
static void lcd_shader_565(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data)
{
	volatile uint32_t r; // dummy read value

	nrf_gpio_pin_write(LCD_SELECT,0); // make sure we are talking to lcd
	lcd_window(px,py,hx,hy); // window 
	lcd_command(CMD_RAMWR); // write pixels
	nrf_gpio_pin_write(LCD_COMMAND,1); // data

	// state passed into lcd_shader_transfer by pointers
	int d1=0;

	int xymax=hx*hy; // need one more pixel loop than asked for
	for(int xy=0;xy<=xymax;xy++)
	{
		int x=px+(xy%hx);
		int y=py+(xy/hx);
		if(xy==0) // first transfer is read only
		{
			d1=(*pixel)(x,y,data);												// get first pixel at start
		}
		else // other transfers
		{
			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)(((d1>>16)&0xf8)|((d1>>13)&0x07));			// out
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;                    								// in
			r;//hush

			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)(((d1>>3)&0x1f)|((d1>>5)&0x70));			// out
			if(xy!=xymax) { d1=(*pixel)(x,y,data); }							// get pixel before waiting... (IMPORTANT)
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;													// in
			r;//hush
		}
	}
}

// shade a rectangular area with a function
static void lcd_shader_444(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data)
{
	volatile uint32_t r; // dummy read value

	nrf_gpio_pin_write(LCD_SELECT,0); // make sure we are talking to lcd
	lcd_window(px,py,hx,hy); // window 
	lcd_command(CMD_RAMWR); // write pixels
	nrf_gpio_pin_write(LCD_COMMAND,1); // data

	// state passed into lcd_shader_transfer by pointers
	int d1=0;
	int d2=0;

	int xymax=hx*hy; // need one more pixel loop than asked for
	for(int xy=0;xy<=xymax;xy++)
	{
		int x=px+(xy%hx);
		int y=py+(xy/hx);
		if(xy&1) // odd
		{
			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)(((d1>>16)&0xf0)|((d1>>12)&0x0f));			// out
			if(xy!=xymax) { d2=(*pixel)(x,y,data); }							// get pixel before waiting... (IMPORTANT)
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;                    								// in
			r;//hush

			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)(((d1)&0xf0)|((d2>>20)&0x0f));				// out
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;													// in
			r;//hush
		}
		else // even
		{
			if(xy==0) // first
			{
				d1=(*pixel)(x,y,data);												// get first pixel at start
			}
			else
			{
				NRF_SPI0->EVENTS_READY=0;											// ready
				NRF_SPI0->TXD=(uint32_t)(((d2>>8)&0xf0)|((d2>>4)&0x0f));			// out
				if(xy!=xymax) { d1=(*pixel)(x,y,data); }							// get pixel before waiting... (IMPORTANT)
				while(NRF_SPI0->EVENTS_READY==0){;}									// wait
				r=NRF_SPI0->RXD;													// in
				r;//hush
			}
		}
	}
}

// shade a rectangular area with a function
static void lcd_shader_888(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data)
{
	volatile uint32_t r; // dummy read value

	nrf_gpio_pin_write(LCD_SELECT,0); // make sure we are talking to lcd
	lcd_window(px,py,hx,hy); // window 
	lcd_command(CMD_RAMWR); // write pixels
	nrf_gpio_pin_write(LCD_COMMAND,1); // data

	// state passed into lcd_shader_transfer by pointers
	int d1=0;
	int xymax=hx*hy; // need one more pixel than asked for
	for(int xy=0;xy<=xymax;xy++)
	{
		int x=px+(xy%hx);
		int y=py+(xy/hx);
		if(xy==0) // first transfer is read only
		{
			d1=(*pixel)(x,y,data);												// get first pixel at start
		}
		else // other transfers
		{
			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)((d1>>16)&0xff);							// out
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;                    								// in
			r;//hush

			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)((d1>>8)&0xff);								// out
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;													// in
			r;//hush

			NRF_SPI0->EVENTS_READY=0;											// ready
			NRF_SPI0->TXD=(uint32_t)((d1)&0xff);								// out
			if(xy!=xymax) { d1=(*pixel)(x,y,data); }							// get pixel before waiting... (IMPORTANT)
			while(NRF_SPI0->EVENTS_READY==0){;}									// wait
			r=NRF_SPI0->RXD;													// in
			r;//hush
		}
	}
}

static int lcd_color_mode_value=0x444;

// shade a rectangular area with a function
void lcd_shader(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data)
{
	switch(lcd_color_mode_value)
	{
		case 0x444:
			lcd_shader_444(px,py,hx,hy,pixel,data);
		break;
		case 0x565:
			lcd_shader_565(px,py,hx,hy,pixel,data);
		break;
		case 0x888:
			lcd_shader_888(px,py,hx,hy,pixel,data);
		break;
	}
}

// set/get color mode values are : 0x444 , 0x565 , 0x888 or 0x000 to just read.
int lcd_color_mode(int color_mode)
{
	nrf_gpio_pin_write(LCD_SELECT,0);
	switch(color_mode)
	{
		case 0x444:
			lcd_command(CMD_COLMOD); lcd_data(0x63);
			lcd_color_mode_value=0x444;
		break;
		case 0x565:
			lcd_command(CMD_COLMOD); lcd_data(0x65);
			lcd_color_mode_value=0x565;
		break;
		case 0x888:
			lcd_command(CMD_COLMOD); lcd_data(0x66);
			lcd_color_mode_value=0x888;
		break;
	}
	return lcd_color_mode_value;
}

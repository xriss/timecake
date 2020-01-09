

// The size of the LCD

#define LCD_HX 240
#define LCD_HY 240



extern int lcd_setup(void);


extern int lcd_backlight(int bright);

// set/get color mode values are : 0x444 , 0x565 , 0x888 or 0x000 to just read.
extern int lcd_color_mode(int color_mode);

// simple shader callback system to fill screen
extern void lcd_shader(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data);

// set every pixel to the color in data
extern int lcd_shader_color(int x,int y,void *data);

// font structure

struct shader_font {
	const void * name; // required pointer to font data
	int    hx,hy;      // width and height of each glyph
	int    foreground; // foreground color
	int    dropshadow; // dropshadow color
	int    background; // background color
	int    length;     // the length of text
	char * text;       // the string of text to render
	int    ax,mx,dx;   // x=((x+ax)*mx)/dx
	int    ay,my,dy;   // y=((y+ay)*my)/dy
};

// font names
extern const unsigned char funfont_4x8b[];
extern const unsigned short funfont_8x16r[];

// text shaders
extern int shader_textline(int x,int y,struct shader_font *data);

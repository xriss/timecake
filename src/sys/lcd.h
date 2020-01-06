

// The size of the LCD

#define LCD_HX 240
#define LCD_HY 240



extern int lcd_setup(void);


extern int lcd_backlight(int bright);


// simple shader callback system to fill screen
extern void lcd_shader(int px,int py,int hx,int hy,int(*pixel)(int x,int y,void *data),void *data);

// set every pixel to the color in data
extern int lcd_shader_color(int x,int y,void *data);




// hardware reference manual
// https://infocenter.nordicsemi.com/pdf/nRF52832_OPS_v0.6.3.pdf

// button state updated every frame
extern int main_butt;


extern int main_state;
extern int main_state_next;


// global buffers

extern char main_text[32*16];
extern struct shader_font main_lines[16];



// states

extern int main_test(int mode);
extern int main_clock1(int mode);

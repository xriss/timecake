
#define BUTTON_STATE_IS    1
#define BUTTON_STATE_DOWN  2
#define BUTTON_STATE_UP    4

extern int button_state; // you may peek



extern int button_setup(void);

int button_read(void);

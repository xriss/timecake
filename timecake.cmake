

project(timecake C CXX ASM)

set(NRF_PROJECT_NAME timecake)
set(EXECUTABLE_NAME timecake)


list(APPEND LINK_LIBRARIES
)


include_directories(./src)
include_directories(./libfixmath/libfixmath)


add_definitions( -DFIXMATH_NO_CACHE )


list(APPEND SOURCE_FILES

	libfixmath/libfixmath/fix16.c
	libfixmath/libfixmath/fix16_sqrt.c
	libfixmath/libfixmath/fix16_trig.c
	libfixmath/libfixmath/uint32.c
	libfixmath/libfixmath/fix16_exp.c
	libfixmath/libfixmath/fix16_str.c
	libfixmath/libfixmath/fract32.c

)


list(APPEND SOURCE_FILES

  src/sys/saveram_pine.c
  src/sys/clock_pine.c
  src/sys/i2c_pine.c
  src/sys/lcd_pine.c
  src/sys/battery_pine.c
  src/sys/acc_pine.c
  src/sys/heart_pine.c
  src/sys/touch_pine.c
  src/sys/button_pine.c

)

list(APPEND SOURCE_FILES

  src/sys/saveram.c
  src/sys/clock.c
  src/sys/lcd.c
  src/sys/lcd_font.c
  src/sys/battery.c
  src/sys/acc.c
  src/sys/heart.c
  src/sys/touch.c
  src/sys/button.c

  src/main.c
  src/main_test.c
  src/main_clock1.c
  
  art/lenna.c

)

set(INCLUDE_FILES
)


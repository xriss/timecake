

project(timecake C CXX ASM)

set(NRF_PROJECT_NAME timecake)
set(EXECUTABLE_NAME timecake)


include_directories(./src)

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

)

set(INCLUDE_FILES
)


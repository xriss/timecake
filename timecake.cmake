

project(timecake C CXX ASM)

set(NRF_PROJECT_NAME timecake)
set(EXECUTABLE_NAME timecake)


include_directories(./src)

list(APPEND SOURCE_FILES

  src/sys/lcd_pine.c
  src/sys/lcd_font.c
  src/sys/lcd.c

  src/sys/battery_pine.c
  src/sys/battery.c

  src/sys/i2c_pine.c
  src/sys/acc_pine.c
  src/sys/heart_pine.c
  src/sys/touch_pine.c

  src/main.c

)

set(INCLUDE_FILES
)


cmake_minimum_required(VERSION 3.12)

project(pinetime-test C CXX ASM)

# define some variables just for this example to determine file locations
set(NRF_PROJECT_NAME timecake)
#set(NRF_BOARD pca10040)
#set(NRF_SOFTDEVICE s132)

nRF5x_toolchainSetup()
nRF5x_setup()

#nRF5x_addAppScheduler()
#nRF5x_addAppFIFO()
#nRF5x_addAppTimer()
#nRF5x_addAppUART()
#nRF5x_addAppButton()
#nRF5x_addBSP(FALSE FALSE FALSE)
#nRF5x_addAppGpiote()
#nRF5x_addBLEGATT()
#
#nRF5x_addBLEService(ble_lbs)

#nRF5x_addAppUART()

#add_definitions(-DCONFIG_GPIO_AS_PINRESET)
#add_definitions(-DDEBUG)
add_definitions(-DUSE_APP_CONFIG)

include_directories(.)

list(APPEND SOURCE_FILES

  sys/lcd_pine.c
  sys/lcd_font.c
  sys/lcd.c

  sys/battery_pine.c
  sys/battery.c

  sys/i2c_pine.c
  sys/acc_pine.c
  sys/heart_pine.c
  sys/touch_pine.c

  main.c

)

set(INCLUDE_FILES
)

nRF5x_addExecutable(timecake "${SOURCE_FILES}")


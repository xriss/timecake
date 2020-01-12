cmake_minimum_required(VERSION 3.12)

option(SEMIHOSTING "enable printf output !WARNING! code will only run with gdb" )

 
include("timecake.cmake")



if(SEMIHOSTING)
	set(SPECS_LINKER_FLAGS " --specs=rdimon.specs -u _printf_float " )
else()
	set(SPECS_LINKER_FLAGS " --specs=nano.specs -lnosys -u _printf_float " )
endif()


include_directories(
	nr5sdk/config/nrf52832/config/
)


set(NRF_TARGET nrf52)

if (NOT DEFINED ARM_NONE_EABI_TOOLCHAIN_PATH)
	set(ARM_NONE_EABI_TOOLCHAIN_PATH "/usr")
endif ()

set(NRF5_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/nr5sdk")


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PREFIX arm-none-eabi-)

if (NOT DEFINED ARM_NONE_EABI_TOOLCHAIN_BIN_PATH)
    if(MINGW OR CYGWIN OR WIN32)
        set(UTIL_SEARCH_CMD where)
    elseif(UNIX OR APPLE)
        set(UTIL_SEARCH_CMD which)
    endif()
    execute_process(
            COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
            OUTPUT_VARIABLE BINUTILS_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    get_filename_component(ARM_NONE_EABI_TOOLCHAIN_BIN_PATH ${BINUTILS_PATH} DIRECTORY)
endif ()

# Without that flag CMake is not able to pass test compilation check
if (${CMAKE_VERSION} VERSION_EQUAL "3.6.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.6")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
    set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")
endif()

set(TOOLCHAIN_PATH_AND_PREFIX ${ARM_NONE_EABI_TOOLCHAIN_BIN_PATH}/${TOOLCHAIN_PREFIX})

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH_AND_PREFIX}gcc)

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH_AND_PREFIX}c++)

set(CMAKE_AR ${TOOLCHAIN_PATH_AND_PREFIX}ar)
set(CMAKE_RANLIB ${TOOLCHAIN_PATH_AND_PREFIX}ranlib)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH_AND_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PATH_AND_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_SYSROOT ${ARM_NONE_EABI_TOOLCHAIN_BIN_PATH})
set(CMAKE_FIND_ROOT_PATH ${ARM_NONE_EABI_TOOLCHAIN_BIN_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


    # fix on macOS: prevent cmake from adding implicit parameters to Xcode
    set(CMAKE_OSX_SYSROOT "/")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "")

    # language standard/version settings
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_CXX_STANDARD 98)

    # CPU specyfic settings

	if(NOT DEFINED NRF5_LINKER_SCRIPT)
		set(NRF5_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/timecake.ld")
	endif()
	set(CPU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
	add_definitions(-DNRF52 -DNRF52832 -DNRF52832_XXAA -DNRF52_PAN_74 -DNRF52_PAN_64 -DNRF52_PAN_12 -DNRF52_PAN_58 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_15 -DNRF52_PAN_20 -DNRF52_PAN_55 -DBOARD_PCA10040)
	add_definitions(-DSOFTDEVICE_PRESENT -DS132 -DSWI_DISABLE0 -DBLE_STACK_SUPPORT_REQD -DNRF_SD_BLE_API_VERSION=6)
	include_directories(
			"${NRF5_SDK_PATH}/components/softdevice/s132/headers"
			"${NRF5_SDK_PATH}/components/softdevice/s132/headers/nrf52"
	)
	list(APPEND SDK_SOURCE_FILES
			"${NRF5_SDK_PATH}/modules/nrfx/mdk/system_nrf52.c"
			"${NRF5_SDK_PATH}/modules/nrfx/mdk/gcc_startup_nrf52.S"
			)
	set(SOFTDEVICE_PATH "${NRF5_SDK_PATH}/components/softdevice/s132/hex/s132_nrf52_6.1.1_softdevice.hex")


    set(COMMON_FLAGS "-MP -MD -mthumb -mabi=aapcs -Wall -g3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums ${CPU_FLAGS}")

    # compiler/assambler/linker flags
    set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O1")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_ASM_FLAGS "-MP -MD -std=c99 -x assembler-with-cpp")
    set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mabi=aapcs -std=gnu++98 -std=c99 -L ${NRF5_SDK_PATH}/modules/nrfx/mdk -T${NRF5_LINKER_SCRIPT} ${CPU_FLAGS} -Wl,--gc-sections -lc ${SPECS_LINKER_FLAGS}")
    # note: we must override the default cmake linker flags so that CMAKE_C_FLAGS are not added implicitly
    set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES> -lm")
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -lstdc++ -o <TARGET> <LINK_LIBRARIES> -lm")

    # basic board definitions and drivers
    include_directories(
            "${NRF5_SDK_PATH}/components"
            "${NRF5_SDK_PATH}/components/boards"
            "${NRF5_SDK_PATH}/components/softdevice/common"
            "${NRF5_SDK_PATH}/integration/nrfx"
            "${NRF5_SDK_PATH}/integration/nrfx/legacy"
            "${NRF5_SDK_PATH}/modules/nrfx"
            "${NRF5_SDK_PATH}/modules/nrfx/drivers/include"
            "${NRF5_SDK_PATH}/modules/nrfx/hal"
            "${NRF5_SDK_PATH}/modules/nrfx/mdk"
    )

    # toolchain specific
    include_directories(
            "${NRF5_SDK_PATH}/components/toolchain/cmsis/include"
    )


    # libraries
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/atomic"
            "${NRF5_SDK_PATH}/components/libraries/atomic_fifo"
            "${NRF5_SDK_PATH}/components/libraries/atomic_flags"
            "${NRF5_SDK_PATH}/components/libraries/balloc"
            "${NRF5_SDK_PATH}/components/libraries/bootloader/ble_dfu"
            "${NRF5_SDK_PATH}/components/libraries/cli"
            "${NRF5_SDK_PATH}/components/libraries/crc16"
            "${NRF5_SDK_PATH}/components/libraries/crc32"
            "${NRF5_SDK_PATH}/components/libraries/crypto"
            "${NRF5_SDK_PATH}/components/libraries/csense"
            "${NRF5_SDK_PATH}/components/libraries/csense_drv"
            "${NRF5_SDK_PATH}/components/libraries/delay"
            "${NRF5_SDK_PATH}/components/libraries/ecc"
            "${NRF5_SDK_PATH}/components/libraries/experimental_section_vars"
            "${NRF5_SDK_PATH}/components/libraries/experimental_task_manager"
            "${NRF5_SDK_PATH}/components/libraries/fds"
            "${NRF5_SDK_PATH}/components/libraries/fstorage"
            "${NRF5_SDK_PATH}/components/libraries/gfx"
            "${NRF5_SDK_PATH}/components/libraries/gpiote"
            "${NRF5_SDK_PATH}/components/libraries/hardfault"
            "${NRF5_SDK_PATH}/components/libraries/hci"
            "${NRF5_SDK_PATH}/components/libraries/led_softblink"
            "${NRF5_SDK_PATH}/components/libraries/log"
            "${NRF5_SDK_PATH}/components/libraries/log/src"
            "${NRF5_SDK_PATH}/components/libraries/low_power_pwm"
            "${NRF5_SDK_PATH}/components/libraries/mem_manager"
            "${NRF5_SDK_PATH}/components/libraries/memobj"
            "${NRF5_SDK_PATH}/components/libraries/mpu"
            "${NRF5_SDK_PATH}/components/libraries/mutex"
            "${NRF5_SDK_PATH}/components/libraries/pwm"
            "${NRF5_SDK_PATH}/components/libraries/pwr_mgmt"
            "${NRF5_SDK_PATH}/components/libraries/queue"
            "${NRF5_SDK_PATH}/components/libraries/ringbuf"
            "${NRF5_SDK_PATH}/components/libraries/scheduler"
            "${NRF5_SDK_PATH}/components/libraries/sdcard"
            "${NRF5_SDK_PATH}/components/libraries/slip"
            "${NRF5_SDK_PATH}/components/libraries/sortlist"
            "${NRF5_SDK_PATH}/components/libraries/spi_mngr"
            "${NRF5_SDK_PATH}/components/libraries/stack_guard"
            "${NRF5_SDK_PATH}/components/libraries/strerror"
            "${NRF5_SDK_PATH}/components/libraries/svc"
            "${NRF5_SDK_PATH}/components/libraries/timer"
            "${NRF5_SDK_PATH}/components/libraries/twi_mngr"
            "${NRF5_SDK_PATH}/components/libraries/twi_sensor"
            "${NRF5_SDK_PATH}/components/libraries/usbd"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/audio"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/cdc"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/cdc/acm"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/hid"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/generic"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/kbd"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/mouse"
            "${NRF5_SDK_PATH}/components/libraries/usbd/class/msc"
            "${NRF5_SDK_PATH}/components/libraries/util"
    )
    
# executable
add_executable(${EXECUTABLE_NAME} ${SDK_SOURCE_FILES} ${SOURCE_FILES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".out")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "-Wl,-Map=${EXECUTABLE_NAME}.map")

# additional POST BUILD setps to create the .bin and .hex files
add_custom_command(TARGET ${EXECUTABLE_NAME}
		POST_BUILD
		COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_NAME}.out
		COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE_NAME}.out "${EXECUTABLE_NAME}.bin"
		COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_NAME}.out "${EXECUTABLE_NAME}.hex"
		COMMENT "post build steps for ${EXECUTABLE_NAME}")





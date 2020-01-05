cmake_minimum_required(VERSION 3.12)

set(NRF_TARGET nrf52)

if (NOT DEFINED ARM_NONE_EABI_TOOLCHAIN_PATH)
	set(ARM_NONE_EABI_TOOLCHAIN_PATH "/usr")
endif ()

set(NRF5_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/nr5sdk")
set(NRFJPROG "${CMAKE_CURRENT_LIST_DIR}/nrfjprog")

include("${CMAKE_CURRENT_LIST_DIR}/CMake_nRF5x.cmake")

nRF5x_toolchainSetup()

project(ExampleProject C)

add_subdirectory(src)

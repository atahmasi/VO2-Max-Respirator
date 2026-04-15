# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion RISCV_ZCB_RPI_2_2_0_3)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(VO2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(VO2
        VO2.c
        functions.c
        ble.c
        dsp.c
        spi_mcp3204.c
        isr.c
)

target_link_libraries(VO2 
            pico_stdlib
            pico_btstack_ble
            pico_btstack_cyw43
            pico_cyw43_arch_none
            hardware_adc
            hardware_spi
            hardware_timer
            pico_multicore
            )

# enable usb output, disable uart output
pico_enable_stdio_usb(VO2 1)
pico_enable_stdio_uart(VO2 0)

target_include_directories(VO2 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # For our common btstack config
    )
pico_btstack_make_gatt_header(VO2 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/VO2.gatt")

# create map/bin/hex file etc.
pico_add_extra_outputs(VO2)

# add url via pico_set_program_url
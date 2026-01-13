# ============================================================
# Author:      Leon Wandruschka
# Date:        01-10-2025
# Description: CMake configuration for the Blink project.
#              Defines sources, Pico SDK libraries, and
#              generates the files for deployment.
# ============================================================

cmake_minimum_required(VERSION 3.12)

############################################################
##                   STUDENT TODO START                   ##
############################################################

#TODO: Change Blink to your project name
project(group-bikeblinker C CXX ASM)

#TODO: Change the files to be included in your project 
set(SOURCE_FILES
        # Place your source files here:
        main.c
        adc.c
        buttons.c
        gpio.c
        hall.c 
        i2c.c
        led.c
        oled.c 
        pwm.c 
        utils.c
)

#TODO: if you use libraries for the Pico, add them here
set(LIB_SOURCES
    # Place your libraries here:
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_pwm
)


############################################################
##                    STUDENT TODO END                    ##
############################################################


# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME} ${LIB_SOURCES})

# Add local includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Force UF2 and other artifacts into the top-level build dir
set(PICO_DEFAULT_BINARY_DIR ${CMAKE_BINARY_DIR})

# Generate the uf2 file
pico_add_extra_outputs(${PROJECT_NAME})

if (NOT ${PICO_NO_PICOTOOL})
add_custom_target(copy_uf2 ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.uf2
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.uf2
COMMENT "Copying ${PROJECT_NAME}.uf2 to top-level build directory"
)
add_dependencies(copy_uf2 ${PROJECT_NAME})
endif ()

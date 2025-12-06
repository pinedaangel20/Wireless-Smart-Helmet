# ============================================================
# Author:      Leon Wandruschka
# Date:        01-10-2025
# Description: Top-level CMake configuration for the
#              Microcontroller project using the Pico SDK.
#              This file sets up toolchain paths, output
#              directories, and includes all project sources.
# ============================================================

cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Set board config. All supported boards can be found in .pico-sdk/src/boards/include
set(PICO_BOARD pico)

# Pull in PICO SDK
include(pico_sdk_import.cmake)

# Setup the project
project(Microcontroller C CXX ASM)

# Create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put ALL targets' outputs into the top-level build/ dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Initialize the SDK
pico_sdk_init()
set(CMAKE_BUILD_TYPE Debug)

# Detect host system and compiler sysroot
message(STATUS "Host System Name: ${CMAKE_HOST_SYSTEM_NAME}")
# Detect ARM GCC system root differently based on OS
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    # On Linux, set ARM_GCC_SYSROOT manually
    set(ARM_GCC_SYSROOT "/usr/lib/arm-none-eabi")
else()
    # On macOS or Windows, detect sysroot automatically
    execute_process(
        COMMAND arm-none-eabi-gcc -print-sysroot
        OUTPUT_VARIABLE ARM_GCC_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
message(STATUS "Detected ARM GCC sysroot: ${ARM_GCC_SYSROOT}")

include_directories(SYSTEM ${ARM_GCC_SYSROOT}/include)
add_compile_options(-I${ARM_GCC_SYSROOT}/include)

# Include subdirectories
add_subdirectory(src)

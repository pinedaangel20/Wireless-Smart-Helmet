set(PICO_SDK_PATH "../.pico-sdk")
set(PICO_NO_PICOTOOL false)

get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${PICO_SDK_PATH})
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' not found")
endif ()

set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake)
if (NOT EXISTS ${PICO_SDK_INIT_CMAKE_FILE})
    message(WARNING "Unable to access Pico SDK at ${PICO_SDK_PATH}. Please run \"git submodule update --init --recursive\" to fetch the SDK.")
    return()
endif ()

include(${PICO_SDK_INIT_CMAKE_FILE})

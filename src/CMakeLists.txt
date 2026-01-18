project(group-bikeblinker C CXX ASM)

# ---------------------------------------------------------
# DEFINICIÓN DE ARCHIVOS FUENTE
# ---------------------------------------------------------
set(SOURCES
    # Archivos del Manillar
    handlebar/handlebar_main.c
    handlebar/buttons.c
    handlebar/hall.c
    
    # Drivers (Hardware)
    drivers/adc.c
    drivers/gpio.c
    drivers/i2c.c
    drivers/led.c
    drivers/oled.c
    drivers/pwm.c
    
    # Archivos compartidos
    shared/utils.c
)

# Librerías del SDK necesarias
set(PICO_LIBS
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_pwm
    hardware_i2c
)

# ---------------------------------------------------------
# CREACIÓN DEL EJECUTABLE
# ---------------------------------------------------------

add_executable(handlebar_firmware ${SOURCES})

# Vincular las librerías
target_link_libraries(handlebar_firmware ${PICO_LIBS})

# Decirle dónde buscar los archivos .h (Header files)
target_include_directories(handlebar_firmware PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/handlebar
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
)

# Habilitar salida USB (para debug con printf)
pico_enable_stdio_usb(handlebar_firmware 1)
pico_enable_stdio_uart(handlebar_firmware 0)

# Generar el archivo .uf2 final
pico_add_extra_outputs(handlebar_firmware)
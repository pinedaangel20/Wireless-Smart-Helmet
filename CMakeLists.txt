cmake_minimum_required(VERSION 3.12)

# =========================================================
# CONFIGURACIÓN DEL SDK
# =========================================================
# Le decimos dónde está el SDK en tu Windows
set(PICO_SDK_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

# =========================================================
# SOLUCIÓN AL ERROR DE WINDOWS
# =========================================================
# Esto evita que CMake intente agregar banderas de Windows (.exe/dll)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Importar SDK
include(pico_sdk_import.cmake)

# Definir proyecto
project(group-bikeblinker C CXX ASM)

# Inicializar SDK
pico_sdk_init()

# Ir a la carpeta src
add_subdirectory(src)
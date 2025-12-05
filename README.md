# Microcontroller Project

This repository provides a **base project structure** for Raspberry Pi Pico (RP2040) development.
It includes a simple CMake configuration and example source files (`main.c`, `led.c`) to help you get started quickly.

## Project Structure

```
.
├── CMakeLists.txt          # Top-level CMake configuration
├── pico_sdk_import.cmake   # Pico SDK import
├── openocd.cfg             # Debug configuration (OpenOCD)
├── README.md               # This file
└── src
    ├── CMakeLists.txt      # Project-specific build settings
    ├── main.c              # Example entry point
    ├── led.c               # Example LED implementation
    └── led.h               # Example LED header
```

## Getting Started

1. **Clone the repository**

2. **Initialize the submodules**

    ```bash
    git submodule update --init --recursive
    ```

3. **Configure your project**  
Open `src/CMakeLists.txt` and update:

    * `project(Blink ...)` → Change `Blink` to your project name.
    * Add your source files to `SOURCE_FILES`.
    * Add required libraries to `LIB_SOURCES` (e.g. `hardware_gpio`, `hardware_pwm`).

4. **Install C compiler (optional)**  
If you have already installed a C compiler, you can skip this step.  
You can check if either gcc or clang is installed by running:
    ```bash
    clang --version
    ```
    ```bash
    gcc --version
    ```
    There are multiuple guides on how to install either on the internet,   
    an example for gcc on Windows 11 is: https://www.youtube.com/watch?v=pTtF6TKJpo8

5. **Build and flash the project**  
    Manually or with the IDE, just like in the assignments before.  

    To flash the project manually (without a debug probe), hold down the bootsel button while plugging in the USB port in your PC. You should now see the mass storage (e.g. RPI-RP2) in the explorer. Simple drag and drop the .uf2 file into the device folder. The device disappears from your explorer and the program will be executed immediatly. You can find your .uf2 file in the build folder in your project after building it. 


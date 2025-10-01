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

    ```bash
    git clone https://github.com/your-username/your-project.git
    cd your-project
    ```

2. **Initialize the submodules**

    ```bash
    git submodule update --init --recursive
    ```

3. **Configure your project**
Open `src/CMakeLists.txt` and update:

    * `project(Blink ...)` → Change `Blink` to your project name.
    * Add your source files to `SOURCE_FILES`.
    * Add required libraries to `LIB_SOURCES` (e.g. `hardware_gpio`, `hardware_pwm`).

4. **Build and flash the project**
    Manually or with the IDE, just like in the assignments before.


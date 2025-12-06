# How to use the Project template

This ready-to-use GitLab repository for the project includes build configurations and example code, allowing you to start building on it right away.

If you want to use this template for personal projects, feel free to fork or clone the [template repository](https://gitlab.tugraz.at/microcontroller/ws25/project-upstream).

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

## Setting the correct board config

You have to specify the correct board config depending on what RP2040/RP2350-based board you use.
To set your the board type, modify line 15 in [CMakeLists.txt](CMakeLists.txt):
```cmake
# Set board config. All supported boards can be found in .pico-sdk/src/boards/include
set(PICO_BOARD pico)
```
Common values:
- `pico` (default) for Raspberry Pi Pico or the student hardware (both v1 and v2)
- `pico_w` for Raspberry Pi Pico W (bluetooth/wifi)
- `pico2` for the newer Raspberry Pi Pico 2
- `pico2_w` for the newer Raspberry Pi Pico 2 W (bluetooth/wifi)

Please remove your build directory (`build`) and reload CMake to ensure that all files have been updated to the new board.

**When using a custom board, you also need to update the [openocd.cfg](openocd.cfg) file to use the correct target configuration:**
- For RP2040-based boards (e.g. Raspberry Pi Pico, Pico W, student hardware v1/v2), use:
```
source [find target/rp2040.cfg]
```
- For RP2350-based boards (e.g. Raspberry Pi Pico 2, Pico 2 W), use:
```
source [find target/rp2350.cfg]
```

## Using picotool instead of the debug probe (optional)

You can use the picotool for programming boards which come without accessible SWD/SWCLK pins or when your project environment makes using the debug probe suboptimal.
The Picotool can also be used to generate UF2 files for manual flashing using the USB bootloader mode.

**Note:**
The picotool can only flash code to a pico, but does not provide debug capabilities. For debugging, you still need a debug probe.

### System requirements for building the picotool

- Working C/C++ compiler for your OS
- Python 3 installation

#### Windows

You can use the Windows package manager `winget` to install MinGW (C/C++ compiler) and Python3:
```sh
winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT.LLVM
winget install -e --id Python.Python.3.11
```

#### Linux (using apt)
```sh
sudo apt install build-essential python3 pkg-config libusb-1.0-0-dev
```

#### macOS
```sh
xcode-select --install
brew install cmake libusb python3
```

### Enabling picotool
1) Make sure to restart your IDE after installing the required tools.
2) Change line 4 in [pico_sdk_import.cmake](pico_sdk_import.cmake) to
```cmake
# Set to false to enable picotool build
set(PICO_NO_PICOTOOL false)
```
3) delete your build directory
4) reload CMake and re-build the project

**The generated UF2 files can be found in the `build` directory.**

## Testing your setup

To verify that everything is configured correctly, build the project and flash it onto the student hardware.

**Note:**
If you are on a custom board, you might need to change the LED gpio pin in the code.

If everything is working, the LED on the hardware should start blinking.  
After this, you can start working on your project using the template.

## Exploring the `src` directory

The `src` directory contains:

- an example project
- a separate `CMakeLists.txt`

### Change the Project Name

Update the default project name `Blink` to match your own project name.  
To do this, modify the [CMakeLists.txt](src/CMakeLists.txt) file inside the `src` directory.
```cmake
project(Blink C CXX ASM)
```
**Do not use spaces in the project name - use underscores `_` instead.**
```cmake
project(My_Project C CXX ASM)
```

### Example Project

The template includes a minimal example project consisting of:

- `main.c` - the program entry point
- `led.h` / `led.c` - basic LED control functions

### Adding or Removing Source Files

All `.c` source files must be listed in the CMake configuration:

```cmake
set(SOURCE_FILES
    main.c
    led.c
)
```

Add your own `.c` files to this list and remove unneeded files.

### Adding Additional Libraries

If you require additional parts of the Pico SDK, add the libraries here:

```cmake
set(LIB_SOURCES
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_pwm
)
```

Several common libraries are already included.

### Changing the `printf` output target

By default, `printf` output is sent over **USB**.  
If you prefer to use **UART**, update the following lines in [src/CMakeLists.txt](src/CMakeLists.txt):

Current configuration (USB):

```cmake
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)
```

To switch to UART:

```cmake
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)
```

**Note:**
If you just use the **D** port on the v2 student hardware, you need to configure printf over UART. 
Otherwise, you need to connect both USB ports to your computer.

## Flashing the code to the microcontroller

There are multiple ways to flash your code to the microcontroller:

### Using the Debug Probe (recommended for student hardware)

If your hardware provides debug headers, you can flash the project using the Debug Probe exactly like in the assignments.

### Flashing the generated UF2 file manually (requires picotool)

If you cannot use the debug probe and want to flash a generated UF2 file manually, you need to follow these steps:

1. Unplug the microcontroller board from the PC
2. Hold the **BOOTSEL** button
3. Plug the microcontroller board back into the PC
4. The board mounts as a mass storage device
5. Drag the generated `.uf2` file from the `build` directory onto the drive
6. The board automatically flashes the new program and unmounts the mass storage device

**Note:**
Since micro USB connectors are fragile, unplug and replug the USB cable on the PC side only, not at the microcontroller side.



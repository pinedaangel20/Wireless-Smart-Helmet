# 🚴‍♂️ Bike Blinker: Wireless Smart Helmet Signaling System

[![Estado del Proyecto](https://img.shields.io/badge/Estado-En%20Desarrollo-orange.svg)](https://gitlab.tugraz.at/microcontroller/ws25/group-bikeblinker)
[![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-blue.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Lenguaje](https://img.shields.io/badge/Lenguaje-C-green.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## Project Description

**Bike Blinker** is an intelligent signaling system developed for the final project of Microcontrollers UE course at TU Graz. The system consists of two independent modules (Handlebar and Helmet) that communicate wirelessly to control turn signals and brake lights, enhancing cyclist safety without requiring them to remove their hands from the handlebar

## Engineering Approach: Bare Metal Development

The core value of this project lies in the complete rejection of the Pico SDK. Every driver was developed by interacting directly with the RP2040 memory-mapped registers, following the official datasheet.

## Hardware Architecture

The system utilizes two Raspberry Pi Pico microcontrollers  with the following peripherals:
  - **Wireless RF Communication**: 2x NRF24L01 radio modules for non-cable functionality
  - **Signaling**: LED Strip (Helmet) and RGB LEDs/OLED (Handlebar)
  - **Sensing**: Hall Effect Sensor with magnet for intelligent brake detection.

## Low-Level Implementation Details

- **I2C & OLED Driver**: Developed a custom graphics engine for the SSD1306. Instead of libraries, it uses direct register writes to the I2C0_BASE (0x40044000)  and implements a manual polling mechanism on the IC_STATUS register to manage hardware FIFOs.
- **ADC Raw Processing**: The brake detection system reads the Hall sensor by configuring the ADC_BASE (0x4004C000). It features a calibrated stabilization delay of 20µs and manual polling of the READY_BIT.
- **Custom PWM Engine**: LED dimming and signaling are handled by direct manipulation of PWM Slice registers (PWM_BASE 0x40050000). The system uses a manual WRAP configuration to define brightness resolution.
- **Precise Timing** (utils.c): Implemented a manual wait_us function using a calibrated nop loop in assembly, calculated for the 125MHz system clock.

# System Logic & FSM

The handlebar module operates using a Finite State Machine (FSM) to ensure deterministic transitions:

1. **BOOT**: Manual initialization of hardware registers.
2. **PAIRING**: Simulated or active radio handshake with LED/OLED visual feedback.
3. **RUNNING**: The main loop monitors inputs (Hall sensor, buttons) and transmits data to the helmet.

### Controls

- **Left Button**: Activates left turn signal
- **Right Button**: Activates right turn signal
- **Brake Sensor**: Automatically detects when brakes are applied (>25% pressure)

### Visual Interface

The OLED display shows contextual information:
- `RIDE`: Normal driving mode
- `<< TURN LEFT`: Left turn signal active
- `TURN RIGHT >>`: Right turn signal active
- `BRAKE!!`: Braking system detected

## Technical Challenges Overcome

- **OLED Synchronization**: Resolved "gibberish" display issues by strictly following the SSD1306 power-on sequence and addressing modes via I2C.
- **Hall Sensor Calibration**: Implemented a percentage-based mapping for the Hall sensor to handle magnetic field inversion and signal noise.
- **Memory Safety**: Utilized atomic clear/set resets for GPIO and I2C registers to prevent race conditions in a bare-metal environment.

## 📁 Project Structure

```
group-bikeblinker/
├── CMakeLists.txt              # Main CMake configuration
├── pico_sdk_import.cmake       # Importing the Pico SDK
├── openocd.cfg                 # OpenOCD configuration
├── README.md                   # This file
├── build/                      # Build files (generated)
└── src/
    ├── CMakeLists.txt          # Project configuration
    ├── handlebar/              # Handlebar-specific code
    │   ├── handlebar_main.c    # Main function
    │   ├── buttons.c/h         # Button control
    │   ├── hall.c/h            # Hall sensor
    │   └── font.h              # Font definitions
    ├── drivers/                # Hardware drivers
    │   ├── adc.c/h             # Direct ADC register access for Hall sensing
    │   ├── gpio.c/h            # SIO and Pad control logic (No-SDK)
    │   ├── i2c.c/h             # Bit-level I2C controller for OLED
    │   ├── led.c/h             # LED control
    │   ├── oled.c/h            # SSD1306 bare-metal graphics primitives
    │   └── pwm.c/h             # Manual PWM slice configuration
    └── shared/                 # Shared utilities
        └── utils.c/h           # Assembly-calibrated timing utilities
```

## Learning Goals & Contributions
- **Developers**: Angel Oswaldo Pineda Ojeda & Esteban Andre Ubieto de León.
- **Achievements**: Successfully manipulated memory at a bare-metal level, implementing industrial concepts from scratch without high-level abstractions.

**Additional Resources**:
- [Raspberry Pi Pico SDK Documentation](https://github.com/raspberrypi/pico-sdk)
- [Proyect Wiki](https://gitlab.tugraz.at/microcontroller/ws25/group-bikeblinker/wikis)

---
#pragma once
#include <stdint.h>

// * PIN CONFIG 
#define LED_PIN 16
#define SWITCH_PIN 18
#define LEFT_BUTTON 20
#define RIGHT_BUTTON 21
// * REGISTERS & OFFSETS 
// SIO
#define SIO_BASE 0xD0000000
#define MYGPIO_IN 0x004 // Input read register
#define GPIO_OUT_SET 0x014 // Atomic bit set
#define GPIO_OUT_CLR 0x018 // Atomic bit clear
#define GPIO_OE_SET 0x024 // Output enable bit set
#define GPIO_OE_CLR 0x028 // Output enable bit clear
// IO_BANK0 
#define IO_BANK0_BASE 0x40014000
#define GPIO0_CTRL 0x004 // Control register for Pin 0
#define GPIO_STRIDE 0x008 // Distance between pins (8 bytes)
// PADS_BANK0 
#define PADS_BANK0_BASE 0x4001C000
#define PADS_GPIO_BASE 0x004 // First PADS register (Pin 0)
// * BITS 
#define IE_BIT 6 // Input Enable bit in PADS register
#define PADS_PDE_BIT 2 // Pull Down Enable bit
#define PADS_PUE_BIT 3 // Pull Up Enable bit
// * FUNCTION CONSTANTS 
#define LOW_LEVEL 0
#define HIGH_LEVEL 1
#define MODE_IN 0 // Input
#define MODE_OUT 1 // Output 
#define PULL_NONE 0 // No resistor
#define PULL_DOWN 1 // Internal Pull-down
#define PULL_UP 2 // Internal Pull-up
#define FUNC_SIO 5 // standard GPIO
#define FUNC_PWM 4 // pin used for pwm
#define FUNC_I2C 3 // pin used for i2c

/**
 * @brief Inicializa los periféricos (LEDs y Botones) con su configuración por defecto
 */
void my_gpio_init(void);

/**
 * @brief Establece el nivel lógico de un pin de salida (High/Low)
 * @param pin Número de pin 
 * @param l   Nivel deseado: HIGH_LEVEL o LOW_LEVEL 
 */
void set_gpio_l(int pin, int l);

/**
 * @brief Lee el nivel lógico actual en el pin físico
 * @param pin Número de pin 
 * @return 1 si hay voltaje, 0 si es tierra, -1 si error.
 */
int get_gpio_l(int pin);

/**
 * @brief Selecciona la función del hardware para un pin específico.
 * @param pin  Número de pin 
 * @param func ID de la función 
 */
void set_gpio_func(int pin, int func);

/**
 * @brief Configura la dirección del pin (Entrada o Salida) y habilita el buffer de entrada
 * @param pin Número de pin 
 * @param m   MODE_IN o MODE_OUT.
 */
void set_gpio_m(int pin, int m);

/**
 * @brief Activa o desactiva las resistencias internas 
 * @param pin Número de pin 
 * @param p   PULL_UP, PULL_DOWN.
 */
void set_gpio_p(int pin, int p);
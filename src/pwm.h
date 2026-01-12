#pragma once
#include <stdint.h>
#include "gpio.h"

// * PWM BASE REGISTER ADDRESS
#define PWM_BASE 0x40050000
// * PWM OFFSETS
#define CSR 0x00 // Control Status Register
#define DIV 0x04 // Frena el reloj. Si no se usa, el PWM va a millones de Hz
#define WRAP 0x10 // TOP - valor maximo del contador -> Define tu "100%". Si pones 1000 aquí, tu brillo irá de 0 a 1000.
#define CC 0x0C // Aquí escribes cuánto brillo quieres ahora mismo. - 0-15 Bits para canal A, 16-31 para B
// * PWM BITS
#define EN_BIT 0 // Arranca el contador del PWM

/**
 * @brief Configura LED para funcion PWM
 */
void pwm_init_led();

/**
 * @brief Cambia el duty cycle para ajustar el brillo del LED
 * @param duty_cycle el nuevo valor de duty cycle
 */
void pwm_set_duty(int value);



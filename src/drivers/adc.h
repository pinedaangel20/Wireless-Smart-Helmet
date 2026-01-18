#pragma once
#include <stdint.h>

#define ADC_BASE 0x4004C000
// * OFFSETS
#define CS 0X00 // CONTROL AND STATUS
// * CS BITS
#define EN_BIT 0 // 1 se enciende el ADC, 0 se apaga
#define START_ONCE_BIT 2 // 1 para medir el voltaje ahora
#define READY_BIT 8 // 1 cuando termina de medir
// * Bits 12-14 AINSEL (Analog Input Select)
#define RESULT 0X04 // se escribe el numero de canal (0-3) para saber que pin medir

/**
 * @brief Busca el registro CS y pone el bit EN en 1 para activar el adc
 */
void my_adc_init();

/**
 * @brief Conecta el motor al pin correcto (HALL -> GPIO26 = canal 0)
 * @param channel canal al que se debe de conectar el pin
 */
void my_adc_select_input(int channel);

/**
 * @brief mide el valor que lee el adc
 * @return result - el valor de layo el adc
 */
int my_adc_read(); 
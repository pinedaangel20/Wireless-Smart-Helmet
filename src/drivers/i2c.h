#pragma once
#include "gpio.h"
#include "stdbool.h"

// * I2C REGISTER BASE ADDRESS
#define I2C0_BASE 0x40044000
// * IC OFFSETS
#define IC_ENABLE 0x6c // para apagar el bloque mientras lo configuramos
#define IC_CON 0x00 // Configuration -> para ponerlo en Master Mode
#define IC_TAR 0x04 // Target Address -> para decirle que le va a hablar al OLED
#define IC_DATA_CMD 0x10 // aqui escribo los bytes que quiero mandar 
#define IC_STATUS 0x70
// * IC BITS
#define IC_TNF_BIT 1 // Transmit FIFO not full, RO: 0 Transmit FIFO is full. 1 Transmit FIFO is not full. 
#define IC_MASTER_MODE_BIT 0 // escribir 1 para activarlo, pero el bit 6 tambien debe de ponerse en 1
#define SPEED_BIT 2
#define IC_SLAVE_DISABLE 6 // para master mode
// * OLED SCREEN DEFINES
#define SDA_PIN 4
#define SCL_PIN 5
#define OLED_ADDR 0x3C

/**
 * @brief Inicializa el bus I2C - Cofigura velocidad y pines
 */
void i2c_init();

/**
 * @brief Envia un byte por el cable y lo escribe dentro del registro IC_DATA_CMD
 * @param val byte a escribir
 */
void i2c_write_byte(int val);


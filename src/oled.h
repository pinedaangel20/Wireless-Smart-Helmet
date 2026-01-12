#pragma once
#include "i2c.h"

// * I2C BASE REGISTER ADDRESS
#define SSD1306_I2C_ADDR 0x3C 
// * BYTES DE CONTROL
#define SSD1306_CONTROL_CMD 0x00 
#define SSD1306_CONTROL_DATA 0x40 
/** COMANDOS */
// * FUNDAMENTALES
#define SSD1306_DISPLAY_OFF 0xAE // Apagar pantalla (sleep mode)
#define SSD1306_DISPLAY_ON 0xAF // Encender pantalla
#define SSD1306_SET_CONTRAST 0x81 // Configurar contraste (brillo)
#define SSD1306_DISPLAY_RAM 0xA4 // Muestra lo que hay en la RAM (Modo normal)
// * DIRECCIONAMIENTO (COmo se guardan los pixels)
#define SSD1306_MEMORY_MODE 0x20 
#define SSD1306_COLUMN_ADDR 0x21 // Configurar rango de columnas
#define SSD1306_PAGE_ADDR 0x22 // Configurar rango de páginas
// * CONFIGURACIÓN DE HARDWARE 
#define SSD1306_SET_START_LINE 0x40 // Línea de inicio de la RAM (0)
#define SSD1306_SEG_REMAP_OP 0xA1 // Mapeo de columna 127 a SEG0 (Flip H)
#define SSD1306_SEG_REMAP_NORM 0xA0 // Mapeo de columna 0 a SEG0
#define SSD1306_SET_MULTIPLEX 0xA8 // Ratio de multiplexado (Altura - 1)
#define SSD1306_COM_SCAN_INV 0xC8 // Escaneo de COM[N-1] a COM0 (Flip V)
#define SSD1306_COM_SCAN_NORM 0xC0 // Escaneo de COM0 a COM[N-1]
#define SSD1306_SET_DISPLAY_OFF 0xD3 // Offset vertical (desplazamiento)
#define SSD1306_SET_COMPINS 0xDA // Configuración de pines COM (Hardware)
// * TIMING Y ENERGIA
#define SSD1306_SET_CLOCK_DIV 0xD5 // Divisor de reloj y oscilador
#define SSD1306_SET_PRECHARGE 0xD9 // Periodo de precarga
#define SSD1306_SET_VCOM_DESEL 0xDB // Nivel de deselección VCOMH
#define SSD1306_CHARGE_PUMP 0x8D // Configuración de la bomba de carga interna
// * DIMENCIONES DE PANTALLA
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

// inicializa la pantalla oled con la secuencia de arranque
/**
 * @brief Inicializa la pantalla OLED con la secuencia de arranque
 */
void oled_init();

/**
 * @brief Limpia la pantalla desde 0,0 
 */
void oled_clear();

/**
 * @brief Renderiza en la pantalla desde 0,0 - usando un buffer array del oled
 */
void oled_render();

/**
 * @brief Configura la pantalla enviando un byte de control de comando seguido 
 * del comando real
 * @param cmd comando real
 */
void oled_send_command(int cmd);

/**
 * @brief Dice los pixeles a pintar enviando un byte de control de datos graficos 
 * seguido del byte de pixel
 * @param byte byte de pixel
 */
void oled_send_data(int byte);

/**
 * @brief Calcula la posicion del pixel por pintar y pinta el pixel en la pantalla
 * @param x posicion x en la pantalla
 * @param y posicion y en la pantalla
 * @param color 1 si se pinta, 0 si no
 */
void oled_set_pixel(int x, int y, int color);
#pragma once
#include "gpio.h"
#include "stdbool.h"
#include "utils.h"

/**
 * @brief Indica si el boton fue presionado tras un corto debounce para limpiar lectura basura
 * @param pin Boton que queremos saber si fue presionado
 * @return true si ambas lecturas luego del debounce son iguales
 * @return true si hay discrepancias entre las lecturas
 */
bool buttonIsPressed(int pin);
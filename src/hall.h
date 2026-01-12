#pragma once 
#include "adc.h"

// * VALORES MAX Y MINIMO DEL HALL
// TODO cambiar por valores correctos al conectar los componentes
#define HALL_MIN_VAL 10
#define HALL_MAX_VAL 1000

/**
 * @brief Calcula el porcentaje actual relativo al freno y
 *        el sensor para saber si estamos frenando o no
 * @return percentage
 */
float read_break_percentage();
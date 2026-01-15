#pragma once
#include "gpio.h"
#include "pwm.h"
#include "utils.h"
#include "stdio.h"

#define MAX_LEVEL 1000 // maximo nivel de brillo
#define STRIDE 100 // zancada por dimming

/**
 * @brief Incrementa y decrementa el brillo de una LED poco a poco 
 */
void gradualLedDimmingAndBrighting();

/**
 * @brief Enciende el LED
 */
void turnLedOn();

/**
 * @brief Apaga el LED
 */
void turnLedOff();

/**
 * @brief Invierte el valor de la corriente del LED
 */
void ledToggle();
#pragma once
#include <stdint.h>

typedef enum
{
  BLINKER_OFF = 0,
  BLINKER_LEFT,
  BLINKER_RIGHT,
  BLINKER_HAZARD
} BlinkerState;

void blinkerInit(void);
void blinkerSetState(BlinkerState state);
void blinkerUpdate(uint32_t now_ms);

#ifndef BLINKER_H
#define BLINKER_H

#include <stdint.h>

typedef enum
{
  BLINKER_OFF,
  BLINKER_LEFT,
  BLINKER_RIGHT,
  BLINKER_HAZARD
} BlinkerState;

void blinkerInit(void);
void blinkerSetState(BlinkerState state);
void blinkerUpdate(uint32_t now_ms);

#endif

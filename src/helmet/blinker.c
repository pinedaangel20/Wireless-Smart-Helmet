#include "blinker.h"
#include "led.h"

#include <stdbool.h>
#include <stdint.h>

#define BLINK_PERIOD_MS 500u

static BlinkerState current_state = BLINKER_OFF;
static uint32_t last_toggle_ms = 0;
static bool phase_on = false;

void blinkerInit(void)
{
  current_state = BLINKER_OFF;
  last_toggle_ms = 0;
  phase_on = false;
  turnLedOff();
}

void blinkerSetState(BlinkerState state)
{
  if (state == current_state)
  {
    return;
  }

  current_state = state;
  last_toggle_ms = 0;
  phase_on = false;
  turnLedOff();
}

void blinkerUpdate(uint32_t now_ms)
{
  if (current_state == BLINKER_OFF)
  {
    turnLedOff();
    return;
  }

  if (last_toggle_ms == 0)
  {
    last_toggle_ms = now_ms;
    phase_on = true;
    turnLedOn();
    return;
  }

  if ((now_ms - last_toggle_ms) >= BLINK_PERIOD_MS)
  {
    last_toggle_ms = now_ms;
    phase_on = !phase_on;

    if (phase_on)
    {
      turnLedOn();
    }
    else
    {
      turnLedOff();
    }
  }
}

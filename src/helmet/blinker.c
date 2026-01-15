#include <stdbool.h>
#include <stdint.h>
#include "blinker.h"


static BlinkerState current_state = BLINKER_OFF;
static bool led_on = false;
static uint32_t last_toggle = 0;

void blinkerUpdate(uint32_t now_ms)
{
  if (current_state == BLINKER_OFF)
  {
    ledAllOff();
    return;
  }

  if (now_ms - last_toggle >= 500)
  {
    led_on = !led_on;
    last_toggle = now_ms;

    if (current_state == BLINKER_LEFT)
      ledSetLeft(led_on);
    else if (current_state == BLINKER_RIGHT)
      ledSetRight(led_on);
  }
}

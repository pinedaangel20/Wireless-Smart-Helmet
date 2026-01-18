#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "pwm.h"
#include "time.h"
#include "blinker.h"
#include "funkmodul.h"

static BlinkerState decodeCmd(uint8_t cmd)
{
  switch (cmd)
  {
    case 1: return BLINKER_LEFT;
    case 2: return BLINKER_RIGHT;
    case 3: return BLINKER_HAZARD;
    case 0:
    default: return BLINKER_OFF;
  }
}

int main(void)
{
  gpio_init();
  pwm_init_led();
  funkmodul_init();   // luego se vuelve NRF24 real
  blinkerInit();

  while (1)
  {
    uint8_t cmd = 0;
    if (funkmodul_try_receive(&cmd))
    {
      blinkerSetState(decodeCmd(cmd));
    }

    blinkerUpdate(timeMs());
  }
}

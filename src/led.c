#include "led.h"

void gradualLedDimmingAndBrighting()
{
  pwm_init_led(); 

  int decrease = MAX_LEVEL;
  int increase = 0;

  // dimm
  for (size_t i = 0; i <= STRIDE; i += 10)    
  {
    pwm_set_duty(decrease);
    decrease -= STRIDE;
    wait_us(10000); 
  }
  // bright
  for (size_t i = 0; i < STRIDE; i += 10)
  {
    pwm_set_duty(increase);
    increase += STRIDE;
    wait_us(10000); 
  }
}
 
void turnLedOn()
{
  set_gpio_l(LED_PIN, HIGH_LEVEL);
}

void turnLedOff()
{
  set_gpio_l(LED_PIN, LOW_LEVEL);
}

void ledToggle()
{
  int first_level = get_gpio_l(LED_PIN);
  int new_level = !first_level;
  set_gpio_l(LED_PIN, new_level);
}




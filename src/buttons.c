#include "buttons.h"

typedef struct
{
  int pin;
  bool pressed;
} Button;

bool buttonIsPressed(int pin)
{
  Button button;
  button.pin = pin;
  bool first_read = get_gpio_l(button.pin);
  wait_us(20000); // debounce
  button.pressed = get_gpio_l(button.pin);

  bool same_reading = first_read == button.pressed;

  // compare after debounce
  if (same_reading && (first_read == 0) && (button.pressed == 0)) // PULL UP entonces al presionarse son 0
  {
    return true; 
  }
  
  return false;
}
#include "hall.h"

float read_break_percentage()
{
  int raw_value = my_adc_read();

  if (raw_value < HALL_MIN_VAL)
  {
    raw_value = HALL_MIN_VAL;
  }
  else if (raw_value > HALL_MAX_VAL)
  {
    raw_value = HALL_MAX_VAL;
  }
  
  int moved = raw_value - HALL_MIN_VAL; // cuanto se movio desde la pocision 0 hasta la posicion actual
  int range = HALL_MAX_VAL - HALL_MIN_VAL;
  float percentage = ((float)moved / range) * 100.f;

  if (percentage >= 100.0)
  {
    return 100.0;
  }
  else if (percentage <= 0.0)
  {
    return 0.0;
  }
  
  return percentage;
}

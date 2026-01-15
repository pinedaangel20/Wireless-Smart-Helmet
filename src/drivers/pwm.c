#include "pwm.h"

volatile uint32_t* pwm_reg_base = (volatile uint32_t*)PWM_BASE;

void pwm_init_led()
{
  set_gpio_func(LED_PIN, FUNC_PWM); // confugira a pwm el led

  // Para LED (GP16) -> Slice 0 y Canal A
  volatile uint32_t* slice_reg = pwm_reg_base; // como usamos slice 0 no necesitamos movernos, simplemente apuntar el pwm reg 

  volatile uint32_t* wrap_reg = slice_reg + (WRAP >> 2);
  *wrap_reg = 1000; // los niveles de brillo que va a tener el LED

  volatile uint32_t* div_reg = slice_reg + (DIV >> 2);
  *div_reg = 16 << 4;

  volatile uint32_t* cc_reg = slice_reg + (CC >> 2);
  *cc_reg = 0;

  uint32_t value = *slice_reg; // CRS es 0x00 entonces empieza justo en la base
  value |= (1u << EN_BIT);
  *slice_reg = value;
}

void pwm_set_duty(int duty_cycle)
{

  volatile uint32_t* slice_reg = pwm_reg_base;
  volatile uint32_t* cc_reg = slice_reg + (CC >> 2);
  *cc_reg = duty_cycle; // settea el register CC al nuevo valor
}

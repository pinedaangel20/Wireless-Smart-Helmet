#include <stdio.h>

#include "led.h"
#include "adc.h"
#include "buttons.h"
#include "gpio.h"
#include "hall.h"
#include "i2c.h"
#include "led.h"
#include "oled.h"
#include "pwm.h"
#include "utils.h"

typedef enum
{
  STATE_BOOT,
  STATE_PAIRING,
  STATE_CONNECTED,
  STATE_RUNNING
} system_state;

int main()
{
  // configurar botones y switch
  gpio_init();
  // inicializar adc
  adc_init();
  // inicializar LED para funcion pwm
  pwm_init_led();
  // inicializar bus y oled con secuencia de arranque
  i2c_init();
  oled_init();
  oled_clear();
  // state inicial del programa
  system_state current_state = STATE_BOOT; 
  uint64_t last_time = 0;

  while (1)
  {
    switch (current_state)
    {
    case STATE_BOOT:
      // HELLO
      wait_us(2000); 
      current_state = STATE_PAIRING;
      break;
    case STATE_PAIRING: 
      // PAIRING...
      // blink leds
      // si el funk modul se conecta: 
      current_state = STATE_RUNNING;
    case STATE_CONNECTED: 
      // CONNECTED
      wait_us(1000);
      // LED FIJO
      current_state = STATE_RUNNING;
    case STATE_RUNNING:
      // led se mantiene fijo entonces no hay que hacer nada
      // Leer botones
      // leer freno
      // leer bateria
      // actualizar pantalla
    default:
      break;
    }
  }
  
}

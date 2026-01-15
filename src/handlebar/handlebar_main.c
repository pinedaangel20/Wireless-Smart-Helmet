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
#include "util.h"

#define BOOT_DELAY_US 2000000 // 2 segundos
#define PAIRING_DELAY_US 500000 // 0.5 segundos
#define CONNECTED_DELAY_US 1500000 // 1.5 segundos 
#define LOOP_DELAY_US 50000 // 50 ms 

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
  
  int last_state = -1; // para actualizar pantalla
  int dots = 0;

  while (1)
  {
    switch (current_state)
    {
      case STATE_BOOT:
        oled_set_cursor(30, 3); // Centrado aprox
        oled_print("HELLO");
        wait_us(BOOT_DELAY_US); 
        oled_clear();
        current_state = STATE_PAIRING;
        break;

      case STATE_PAIRING: 
        oled_set_cursor(20, 3);
        oled_print("PAIRING");

        bool connected;
        // TODO FUNKMODUL LOGIC to connect
        for(int i=0; i< dots; i++) 
        {
          oled_draw_char('.');
        }
        // Borramos los puntos sobrantes con espacios si se reinicia
        if(dots == 0) 
        {
          oled_print("   "); 
        }

        dots++;
        if (dots > 3)
        { 
          dots = 0;
        }

        ledToggle(); 
        wait_us(PAIRING_DELAY_US);

        if (connected) 
        { 
          current_state = STATE_CONNECTED;
        }
        break;

      case STATE_CONNECTED: 
        oled_clear();
        oled_set_cursor(20, 3);
        oled_print("CONNECTED!");
        wait_us(CONNECTED_DELAY_US);
        turnLedOn();
        current_state = STATE_RUNNING;
        break;

      case STATE_RUNNING:

        float brake_percentage = read_break_percentage();
        int breaking = (brake_percentage > 25.0f);
        int left = (get_gpio_l(LEFT_BUTTON) << 2);
        int right = (get_gpio_l(RIGHT_BUTTON) << 1);
        
        int state = (breaking << 2) | (left << 1) | (right);

        // actualizar la pantalla si algo cambió 
        if (state != last_state) 
        {
          oled_clear(); // Borramos solo al cambiar de estado
          oled_set_cursor(0, 2); 

          if (breaking) 
          {
            oled_set_cursor(30, 2); // Centrar un poco
            oled_print("BRAKE!!");
          } 
          else if (left) 
          {
            oled_print("<< TURN LEFT");
          } 
          else if (right) 
          {
            oled_print("TURN RIGHT >>");
          } 
          else 
          {
            oled_set_cursor(40, 2);
            oled_print("RIDE"); 
          }
            
          last_state = state; // guardamos memoria
        }
                
        // pausa para no saturar procesador
        wait_us(LOOP_DELAY_US);
        break;
    }
  }
}

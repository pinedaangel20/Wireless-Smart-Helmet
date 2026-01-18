#include "gpio.h"
#include "adc.h"
#include "led.h"
#include "pwm.h"
#include "hall.h"
#include "i2c.h"
#include "oled.h"
#include "utils.h"
#include "funkmodul.h"

// Configuraciones de tiempo
#define BOOT_DELAY_US 2000000 
#define PAIRING_STEP_US 500000 // 0.5 segundos por paso
#define PAIRING_STEPS 10 // 10 pasos = 5 segundos total
#define LOOP_DELAY_US 100000 // 100ms velocidad del bucle principal
#define HALL_ADC_CHANNEL 0 

// Estados del sistema
typedef enum {
    STATE_BOOT,
    STATE_PAIRING,
    STATE_CONNECTED,
    STATE_RUNNING
} system_state;

// protocol codes
#define CODE_IDLE 0
#define CODE_LEFT 1
#define CODE_RIGHT 2
#define CODE_BRAKE 3

void system_shutdown_visuals() 
{
  turnLedOff();
  oled_send_command(SSD1306_DISPLAY_OFF); // apaga pantalla OLED
}

int main() 
{    
  // Inicializar GPIO básico 
  my_gpio_init();

  while(1)
  {
    if (get_gpio_l(SWITCH_PIN) == HIGH_LEVEL) 
    {
      system_shutdown_visuals();
      
      // esperar aqui hasta que el usuario encienda el switch
      while (get_gpio_l(SWITCH_PIN) == HIGH_LEVEL) 
      {
        wait_us(100000); 
      }
      // arrancamos el sistema
    }

    my_adc_init();       
    my_adc_select_input(HALL_ADC_CHANNEL); 
    
    oled_init(); // enciende pantalla y resetea config
    oled_clear();
    
    funkmodulInitTx();// inicializa funkmodul

    // prueba de vida
    for(int i = 0; i < 3; i++)
    {
      turnLedOn();
      wait_us(150000);
      turnLedOff();
      wait_us(150000);
    }

    system_state current_state = STATE_BOOT;
    
    // variables de estado
    int dots = 0;
    int last_switch_state = -1;
    int last_screen_code = -1;
    
    // variable de control para mantener el sistema corriendo
    int system_is_on = 1;

    // bucle principal
    while(system_is_on) 
    {
      if (get_gpio_l(SWITCH_PIN) == HIGH_LEVEL) 
      {
        system_is_on = 0; // rompe este bucle y vuelve al bucle de poder
        break;
      }

      switch(current_state) 
      {
        case STATE_BOOT:
          oled_set_cursor(30, 3);
          oled_print("WELCOME");
          wait_us(BOOT_DELAY_US);
          oled_clear();
          current_state = STATE_PAIRING;
          break;

        case STATE_PAIRING:
          oled_set_cursor(20, 3);
          oled_print("PAIRING");
          
          // bucle de intentos
          for(int i=0; i<PAIRING_STEPS; i++) 
          {
            // chequeo de seguridad por si se apaga el switch durante el pairing
            if (get_gpio_l(SWITCH_PIN) == HIGH_LEVEL) { system_is_on = 0; break; }

            // puntos
            oled_set_cursor(80, 3);
            if(dots == 0) 
            {
              oled_print("   "); // borrar puntos
            }
            if(dots > 0) 
            {
              oled_print(".");
            }
            if(dots > 1) 
            {
              oled_print(".");
            }
            if(dots > 2) 
            {
              oled_print(".");
            }
            
            dots++;
            if(dots > 3) 
            {
              dots = 0;
            }
            
            ledToggle(); 

            // ver si el otro mc nos responde
            if (funkmodulSendByte(CODE_IDLE)) 
            {
              // Si responde, salimos del bucle de intentos antes de tiempo
              break; 
            }

            wait_us(PAIRING_STEP_US);
          }
          
          current_state = STATE_CONNECTED;
          break;

        case STATE_CONNECTED:
          oled_clear();
          oled_set_cursor(10, 3);
          oled_print("CONECTED!");
          turnLedOn(); 
          wait_us(2000000); 
          oled_clear();
          current_state = STATE_RUNNING;
          break;

        case STATE_RUNNING: 
        {
          if(last_switch_state != 1) 
          {
            oled_clear(); 
            last_switch_state = 1;
          }

          float brake_pct = read_break_percentage();
          int is_breaking = (brake_pct > 10.0f); 
          
          int left_pressed = (get_gpio_l(LEFT_BUTTON) == LOW_LEVEL); 
          int right_pressed = (get_gpio_l(RIGHT_BUTTON) == LOW_LEVEL);

          // envia datos por radio
          uint8_t data_to_send = CODE_IDLE;
          if (is_breaking) 
          {
            data_to_send = CODE_BRAKE;
          }
          else if (left_pressed) 
          {
            data_to_send = CODE_LEFT;
          }
          else if (right_pressed) 
          {
            data_to_send = CODE_RIGHT;
          }
          
          funkmodulSendByte(data_to_send);

          int current_code = (is_breaking << 2) | (left_pressed << 1) | right_pressed;

          if(current_code != last_screen_code) 
          {
            oled_clear();
            if(is_breaking) 
            {
              oled_set_cursor(30, 2);
              oled_print("BRAKING!");
            } 
            else if(left_pressed) 
            {
              oled_set_cursor(10, 2);
              oled_print("<< LEFT");
            } 
            else if(right_pressed) 
            {
              oled_set_cursor(10, 2);
              oled_print("RIGHT >>");
            } 
            else 
            {
              oled_set_cursor(40, 2);
              oled_print("RIDE");
            }
            last_screen_code = current_code;
          }

          if (is_breaking) 
          {
            turnLedOn(); 
          } 
          else if (left_pressed || right_pressed) 
          {
            ledToggle(); 
          } 
          else 
          {
            turnLedOn(); 
          }
              
          wait_us(LOOP_DELAY_US);
          break;
        }
      }
    }
  }
  return 0;
}
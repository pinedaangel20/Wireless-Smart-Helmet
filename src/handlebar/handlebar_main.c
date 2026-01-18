#include "gpio.h"
#include "adc.h"
#include "led.h"
#include "pwm.h"
#include "hall.h"
#include "i2c.h"
#include "oled.h"
#include "utils.h"

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

int main() 
{    
    // inicializa LED, Botones y Switch en modo GPIO normal 
    my_gpio_init();      
    // inicializa el ADC y conecta el canal 0 para el Hall
    my_adc_init();       
    my_adc_select_input(HALL_ADC_CHANNEL); 
    // Inicializa pantalla OLED
    oled_init();         
    oled_clear();

    // prueba de vida al encender
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
    int last_switch_state = -1; // -1 fuerza a pintar la pantalla la primera vez
    int last_screen_code = -1; // para no borrar la pantalla si no cambia el mensaje

    while(1) 
    {
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
            
          // Bucle de 10 pasos x 0.5s = 5 segundos
          for(int i=0; i<PAIRING_STEPS; i++) 
          {
            oled_set_cursor(80, 3);
            if(dots == 0) 
            {
              oled_print("   "); // borra puntos viejos
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
            
            ledToggle(); // parpadeo
            wait_us(PAIRING_STEP_US);
          }

          current_state = STATE_CONNECTED;
          break;

        case STATE_CONNECTED:
          oled_clear();
          oled_set_cursor(10, 3);
          oled_print("CONECTED!");
          turnLedOn(); 
          wait_us(2000000); // 2 segundosh
          oled_clear();
          current_state = STATE_RUNNING;
          break;

        case STATE_RUNNING: 
        {
          int switch_val = get_gpio_l(SWITCH_PIN); 
          if(switch_val == HIGH_LEVEL) 
          {
            // MODO APAGADO / STANDBY 
            if(last_switch_state != 0) 
            {
              oled_clear();
              oled_set_cursor(20, 3);
              oled_print("SWITCH OFF");
              turnLedOff();
              last_switch_state = 0;
              last_screen_code = -1; // reset codigo de pantalla
            }
          } 
          else 
          {
            // RIDE
            if(last_switch_state != 1) 
            {
              oled_clear(); // Borrar "SWITCH OFF"
              last_switch_state = 1;
            }

            float brake_pct = read_break_percentage();
            int is_breaking = (brake_pct > 50.0f); // Umbral de freno
            
            int left_pressed = (get_gpio_l(LEFT_BUTTON) == LOW_LEVEL); 
            int right_pressed = (get_gpio_l(RIGHT_BUTTON) == LOW_LEVEL);

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
              ledToggle(); // Intermitente
            } 
            else 
            {
              turnLedOn(); // Luz de posición fija
            }
          }
            
          wait_us(LOOP_DELAY_US);
          break;
        }
      }
    }
  return 0;
}
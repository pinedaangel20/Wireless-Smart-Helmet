#include "gpio.h"

volatile uint32_t* io_bank0_base = (volatile uint32_t*)IO_BANK0_BASE;   
volatile uint32_t* pads_bank0_base = (volatile uint32_t*)PADS_BANK0_BASE; 
volatile uint32_t* sio_base = (volatile uint32_t*)SIO_BASE;        
 
void set_gpio_l(int pin, int l)
{
  if (pin < 0 || pin > 29)
  {
    return;
  }
  else if (l != HIGH_LEVEL && l != LOW_LEVEL)
  {
    return;
  }
  
  uint32_t mask = 1u << pin; // mascara con 1 en la posicion del pin
  // registros atomicos set y clear, es mejor que hacer |= o &= pq evita race conditions
  if (l == HIGH_LEVEL)
  {
    *(sio_base + (GPIO_OUT_SET >> 2)) = mask;
  }
  else
  {
    *(sio_base + (GPIO_OUT_CLR >> 2)) = mask;
  }
}

int get_gpio_l(int pin)
{
  if (pin < 0 || pin > 29)
  {
    return -1;
  }

  uint32_t reg = *(sio_base + (MYGPIO_IN >> 2));
  int input_l = (int)((reg >> pin) & 1u);
  
  return input_l;
}

void set_gpio_func(int pin, int func)
{
  if (pin < 0 || pin > 29)
  {
    return;
  }
  else if (func < 0 || func > 31)
  {
    return;
  }
  
  // base + control del primer pin + salto entre pin y pin (cada uno ocupa 8 bytes -> 4 status y 4 control) * pin
  uint32_t offset_bytes = GPIO0_CTRL + (GPIO_STRIDE * pin);
  volatile uint32_t* ctrl_reg = io_bank0_base + (offset_bytes >> 2);
  uint32_t value = *ctrl_reg;
  value &= ~0x1F; // Limpiamos los 5 bits inferiores (Mask 0001 1111)    
  value |= (func & 0x1F); // escribimos nueva funcion
  *ctrl_reg = value;
}

void set_gpio_m(int pin, int m)
{
  if (pin < 0 || pin > 29)
  {
    return;
  }
  else if (m != MODE_IN && m != MODE_OUT)
  {
    return;
  }
  
  // SIO (un registro para todos los pines)
  uint32_t mask = 1u << pin; // mask basada en el pin, pq el bit depende del pin
  if (m == MODE_OUT) // 1
  {
    // escribimos el registro de set (atomic)
    *(sio_base + (GPIO_OE_SET >> 2)) = mask;
  }
  else // 0
  {
    // escribimos el registro de clear (atomic)
    *(sio_base + (GPIO_OE_CLR >> 2)) = mask;
  }
  
  // PADS (un registro para cada pin)
  uint32_t pads_offset = 0x04 + (4 * pin);
  volatile uint32_t* pad_reg = pads_bank0_base + (pads_offset >> 2);
  uint32_t value = *pad_reg;
  value |= (1u << IE_BIT);
  *pad_reg = value;
}

void set_gpio_p(int pin, int p)
{
  if (pin < 0 || pin > 29)
  {
    return;
  }
  else if (p != PULL_DOWN && p != PULL_UP)
  {
    return;
  }
  
  uint32_t pads_offset = 0x04 + (4 * pin);
  volatile uint32_t* pad_reg = pads_bank0_base + (pads_offset >> 2);
  uint32_t value = *pad_reg;
  value &= ~((1 << PADS_PDE_BIT) | (1 << PADS_PUE_BIT)); //clear both bits

  if (p == PULL_DOWN)
  {
    value |= (1u << PADS_PDE_BIT);
  }
  else
  {
    value |= (1u << PADS_PUE_BIT);
  }
  *pad_reg = value;
}

void my_gpio_init()
{
  // * LED
  set_gpio_func(LED_PIN, FUNC_SIO);
  set_gpio_l(LED_PIN, LOW_LEVEL);
  set_gpio_m(LED_PIN, MODE_OUT);
  set_gpio_p(LED_PIN, PULL_NONE); // empieza apagado
  // * BUTTONS
  set_gpio_func(LEFT_BUTTON, FUNC_SIO);
  set_gpio_m(LEFT_BUTTON, MODE_IN);
  set_gpio_p(LEFT_BUTTON, PULL_UP);
  set_gpio_func(RIGHT_BUTTON, FUNC_SIO);
  set_gpio_m(RIGHT_BUTTON, MODE_IN);
  set_gpio_p(RIGHT_BUTTON, PULL_UP);
  // * SWITCH
  set_gpio_func(SWITCH_PIN, FUNC_SIO);
  set_gpio_m(SWITCH_PIN, MODE_IN);
  set_gpio_p(SWITCH_PIN, PULL_UP);
}

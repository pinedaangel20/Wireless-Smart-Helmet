#include "adc.h"
#include "utils.h"

/* BASE REGISTER POINTER */
static volatile uint32_t* adc_reg_base = (volatile uint32_t*)ADC_BASE;

void adc_init()
{
  volatile uint32_t* reg = (adc_reg_base + (CS >> 2));
  uint32_t value = *reg;
  value |= (1u << EN_BIT);
  *reg = value;
  // esperar un poco luego para que se caliente el adc
  wait_us(20);
}

void adc_select_input(int channel)
{
  if (channel < 0 || channel > 3)
  {
    return;
  } 
  
  uint32_t mask = 0x7000; // para limpiar los bits 12-14
  volatile uint32_t* reg = (adc_reg_base + (CS >> 2));
  uint32_t value = *reg;
  value &= ~mask; // limpair bits
  value |= (channel << 12);
  *reg = value; // escrbir el valor del canal de vuelta en CS
}

int adc_read()
{
  volatile uint32_t* reg = (adc_reg_base + (CS >> 2));
  uint32_t value = *reg;
  value |= (1u << START_ONCE_BIT); // empieza a medir el voltaje en ese momento
  *reg = value;

  uint32_t ready_mask = 1u << READY_BIT; // muevo un 1 hasta la posicion del READY_BIT (8) para ver si luego chocan
  while (1) 
  {
    if (*reg & ready_mask) // si chocan
    {
      break;
    }
  }
  
  uint32_t offset = (RESULT >> 2);
  volatile uint32_t* result_reg = (adc_reg_base + offset);
  uint16_t result = *result_reg;

  return (int)result;
}

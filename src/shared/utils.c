#include "utils.h"

void wait_us(uint32_t time_us)
{
  // tomo el tiempo deseado en micro segundos y lo multiplico por los ciclos de CPU
  uint64_t product = (uint64_t)time_us * (uint64_t)CYCLES_PER_US;

  /*
      tomo el producto del tiempo y los ciclos del cpu y los divido dentro de los 
      ciclos por iteracion que inclye: 
      - coste del for (decremento, comparacion y salto)
      - coste del nop
  */ 
  uint32_t iterations = (uint32_t)(product / CYCLES_PER_ITERATION); 

  for (uint32_t i = iterations; i > 0; i--)
  {
    __asm__ volatile("nop"); // no hace nada pero consume ciclos 
  }
}
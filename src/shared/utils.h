#pragma once
#include <stdint.h>

// 125 MHz ⇒ 125 cycles/µs
#define CYCLES_PER_US 125
// nop ≈ 1, comparison ≈ 1, decrement ≈ 1, bne(branch is no equal) ≈ 3
#define CYCLES_PER_ITERATION 5 

void wait_us(uint32_t time_us);



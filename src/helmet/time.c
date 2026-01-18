#include "time.h"

#define TIMER_BASE 0x40054000u
#define TIMERAWL   0x28u
#define TIMERAWH   0x2Cu

static inline uint32_t mmioRead32(uint32_t addr)
{
  return *(volatile uint32_t *)addr;
}

uint32_t timeMs(void)
{
  uint32_t hi1 = mmioRead32(TIMER_BASE + TIMERAWH);
  uint32_t lo  = mmioRead32(TIMER_BASE + TIMERAWL);
  uint32_t hi2 = mmioRead32(TIMER_BASE + TIMERAWH);

  if (hi1 != hi2)
  {
    lo = mmioRead32(TIMER_BASE + TIMERAWL);
    hi1 = hi2;
  }

  uint64_t us = ((uint64_t)hi1 << 32) | (uint64_t)lo;
  return (uint32_t)(us / 1000u);
}

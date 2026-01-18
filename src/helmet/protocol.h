#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef enum
{
  STATE_OFF    = 0x00,
  STATE_LEFT   = 0x01,
  STATE_RIGHT  = 0x02,
  STATE_HAZARD = 0x03,
  STATE_BRAKE  = 0x04
} DeviceState;

#endif

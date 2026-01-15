#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef enum
{
  STATE_OFF    = 0x00,
  STATE_LEFT   = 0x01,
  STATE_RIGHT  = 0x02,
  STATE_BRAKE  = 0x03,
  STATE_HAZARD = 0x04
} DeviceState;

typedef struct
{
  uint8_t magic;     // 0xB1 por ejemplo
  uint8_t state;     // DeviceState
  uint8_t counter;   // para detectar paquetes repetidos
  uint8_t crc;       // checksum simple
} Packet;

static inline uint8_t packetCrc(const Packet *p)
{
  // checksum simple (no críptico): XOR
  return (uint8_t)(p->magic ^ p->state ^ p->counter);
}

#endif

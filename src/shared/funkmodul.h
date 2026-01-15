#ifndef FUNKMODUL_H
#define FUNKMODUL_H

#include <stdbool.h>
#include <stdint.h>
#include "protocol.h"

void funkmodulInit(void);

// En el casco (tu MCU): intenta leer 1 paquete.
// Devuelve true si recibió uno válido (CRC y magic ok).
bool funkmodulTryReceive(Packet *out_packet);

// En el mando (MCU de tu compa): manda estado
bool funkmodulSendState(DeviceState state);

#endif

#include "funkmodul.h"
#include <string.h>

// ====== LOW LEVEL (DEPENDE DEL MODULO REAL) ======
// Si es UART: usa uart_read/uard_write de tu driver.
// Si es SPI: usa spi_write_read de tu driver.
// Si tu repo ya tiene driver para eso, llama a esas funciones aquí.

static void radioLowLevelInit(void)
{
  // TODO: init del módulo (SPI/UART + pines + config radio)
}

static bool radioLowLevelRead(uint8_t *buf, uint32_t len)
{
  // TODO: leer len bytes (no bloqueante si puedes)
  // return true si leíste len bytes completos
  (void)buf;
  (void)len;
  return false;
}

static bool radioLowLevelWrite(const uint8_t *buf, uint32_t len)
{
  // TODO: mandar len bytes
  (void)buf;
  (void)len;
  return false;
}

// ====== DRIVER “ALTO NIVEL” (NO CAMBIA) ======

static uint8_t tx_counter = 0;

void funkmodulInit(void)
{
  tx_counter = 0;
  radioLowLevelInit();
}

bool funkmodulTryReceive(Packet *out_packet)
{
  Packet p;
  if (!radioLowLevelRead((uint8_t *)&p, sizeof(Packet)))
  {
    return false;
  }

  if (p.magic != 0xB1)
  {
    return false;
  }

  if (p.crc != packetCrc(&p))
  {
    return false;
  }

  *out_packet = p;
  return true;
}

bool funkmodulSendState(DeviceState state)
{
  Packet p;
  p.magic = 0xB1;
  p.state = (uint8_t)state;
  p.counter = tx_counter++;
  p.crc = packetCrc(&p);

  return radioLowLevelWrite((const uint8_t *)&p, sizeof(Packet));
}

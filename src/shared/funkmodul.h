#pragma once
#include <stdbool.h>
#include <stdint.h>

// =======================
// PINOUT (AJUSTAR)
// =======================
// Elegidos para NO chocar con tus pines actuales:
// LED 16, SWITCH 18, botones 19/20, I2C 4/5
// Usa pines libres: 10-14 como ejemplo.
#define NRF_CE_PIN 14 
#define NRF_CSN_PIN 13 
#define NRF_SCK_PIN 10
#define NRF_MOSI_PIN 11
#define NRF_MISO_PIN 12

// =======================
// Radio settings
// =======================
#define NRF_CHANNEL      76   // 0..125
#define NRF_PAYLOAD_LEN  1    // 1 byte

// Address (5 bytes). Debe ser IGUAL en TX y RX.
extern const uint8_t NRF_ADDR[5];

void funkmodulInitTx(void);
void funkmodulInitRx(void);

// TX: manda 1 byte. true si TX_DS, false si MAX_RT
bool funkmodulSendByte(uint8_t b);

// RX: intenta leer 1 byte. true si recibió uno
bool funkmodulTryReceiveByte(uint8_t *out_b);

#include "funkmodul.h"
#include "gpio.h"
#include "utils.h"

const uint8_t NRF_ADDR[5] = { 'B', 'L', 'I', 'N', 'K' };

// -----------------------------
// NRF24 commands
// -----------------------------
#define NRF_CMD_R_REGISTER    0x00
#define NRF_CMD_W_REGISTER    0x20
#define NRF_CMD_R_RX_PAYLOAD  0x61
#define NRF_CMD_W_TX_PAYLOAD  0xA0
#define NRF_CMD_FLUSH_TX      0xE1
#define NRF_CMD_FLUSH_RX      0xE2
#define NRF_CMD_NOP           0xFF

// -----------------------------
// NRF24 registers
// -----------------------------
#define NRF_REG_CONFIG        0x00
#define NRF_REG_EN_AA         0x01
#define NRF_REG_EN_RXADDR     0x02
#define NRF_REG_SETUP_AW      0x03
#define NRF_REG_SETUP_RETR    0x04
#define NRF_REG_RF_CH         0x05
#define NRF_REG_RF_SETUP      0x06
#define NRF_REG_STATUS        0x07
#define NRF_REG_RX_ADDR_P0    0x0A
#define NRF_REG_TX_ADDR       0x10
#define NRF_REG_RX_PW_P0      0x11
#define NRF_REG_FIFO_STATUS   0x17

// -----------------------------
// Bits
// -----------------------------
#define NRF_CONFIG_PRIM_RX    (1u << 0)
#define NRF_CONFIG_PWR_UP     (1u << 1)
#define NRF_CONFIG_EN_CRC     (1u << 3)
#define NRF_CONFIG_CRCO       (1u << 2)  // 1 = 2 bytes CRC

#define NRF_STATUS_RX_DR      (1u << 6)
#define NRF_STATUS_TX_DS      (1u << 5)
#define NRF_STATUS_MAX_RT     (1u << 4)

#define NRF_FIFO_RX_EMPTY     (1u << 0)

// -----------------------------
// Soft-SPI helpers
// -----------------------------
static void spiDelay(void)
{
  // Ajustable si hace falta. Mantén pequeño.
  wait_us(1);
}

static void csnLow(void)  { set_gpio_l(NRF_CSN_PIN, LOW_LEVEL); }
static void csnHigh(void) { set_gpio_l(NRF_CSN_PIN, HIGH_LEVEL); }
static void ceLow(void)   { set_gpio_l(NRF_CE_PIN, LOW_LEVEL); }
static void ceHigh(void)  { set_gpio_l(NRF_CE_PIN, HIGH_LEVEL); }

static uint8_t spiTransfer(uint8_t out)
{
  uint8_t in = 0;

  for (int i = 7; i >= 0; --i)
  {
    // MOSI
    int bit = (out >> i) & 1;
    set_gpio_l(NRF_MOSI_PIN, bit);

    // SCK high
    set_gpio_l(NRF_SCK_PIN, HIGH_LEVEL);
    spiDelay();

    // MISO sample
    in <<= 1;
    in |= (uint8_t)(get_gpio_l(NRF_MISO_PIN) & 1);

    // SCK low
    set_gpio_l(NRF_SCK_PIN, LOW_LEVEL);
    spiDelay();
  }

  return in;
}

// -----------------------------
// NRF register I/O
// -----------------------------
static uint8_t nrfReadReg(uint8_t reg)
{
  csnLow();
  spiTransfer(NRF_CMD_R_REGISTER | (reg & 0x1F));
  uint8_t v = spiTransfer(NRF_CMD_NOP);
  csnHigh();
  return v;
}

static void nrfWriteReg(uint8_t reg, uint8_t val)
{
  csnLow();
  spiTransfer(NRF_CMD_W_REGISTER | (reg & 0x1F));
  spiTransfer(val);
  csnHigh();
}

static void nrfReadBuf(uint8_t reg, uint8_t *buf, uint32_t len)
{
  csnLow();
  spiTransfer(NRF_CMD_R_REGISTER | (reg & 0x1F));
  for (uint32_t i = 0; i < len; i++)
  {
    buf[i] = spiTransfer(NRF_CMD_NOP);
  }
  csnHigh();
}

static void nrfWriteBuf(uint8_t reg, const uint8_t *buf, uint32_t len)
{
  csnLow();
  spiTransfer(NRF_CMD_W_REGISTER | (reg & 0x1F));
  for (uint32_t i = 0; i < len; i++)
  {
    spiTransfer(buf[i]);
  }
  csnHigh();
}

static void nrfCmd(uint8_t cmd)
{
  csnLow();
  spiTransfer(cmd);
  csnHigh();
}

static void nrfClearIrq(void)
{
  // Escribir 1 a RX_DR/TX_DS/MAX_RT las limpia
  nrfWriteReg(NRF_REG_STATUS, NRF_STATUS_RX_DR | NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT);
}

static void nrfPowerUp(void)
{
  uint8_t cfg = nrfReadReg(NRF_REG_CONFIG);
  cfg |= NRF_CONFIG_PWR_UP;
  nrfWriteReg(NRF_REG_CONFIG, cfg);
  // t_pd2stby: ~1.5ms
  wait_us(2000);
}

static void nrfSetRxMode(bool rx)
{
  uint8_t cfg = nrfReadReg(NRF_REG_CONFIG);
  if (rx)
  {
    cfg |= NRF_CONFIG_PRIM_RX;
  }
  else
  {
    cfg &= (uint8_t)~NRF_CONFIG_PRIM_RX;
  }
  nrfWriteReg(NRF_REG_CONFIG, cfg);
}

static void nrfInitPins(void)
{
  // NRF pins as SIO
  set_gpio_func(NRF_CE_PIN, FUNC_SIO);
  set_gpio_func(NRF_CSN_PIN, FUNC_SIO);
  set_gpio_func(NRF_SCK_PIN, FUNC_SIO);
  set_gpio_func(NRF_MOSI_PIN, FUNC_SIO);
  set_gpio_func(NRF_MISO_PIN, FUNC_SIO);

  set_gpio_m(NRF_CE_PIN, MODE_OUT);
  set_gpio_m(NRF_CSN_PIN, MODE_OUT);
  set_gpio_m(NRF_SCK_PIN, MODE_OUT);
  set_gpio_m(NRF_MOSI_PIN, MODE_OUT);
  set_gpio_m(NRF_MISO_PIN, MODE_IN);

  // Idle states
  ceLow();
  csnHigh();
  set_gpio_l(NRF_SCK_PIN, LOW_LEVEL);
  set_gpio_l(NRF_MOSI_PIN, LOW_LEVEL);
}

static void nrfCommonSetup(void)
{
  nrfInitPins();

  // Basic config: CRC enabled, 2 bytes, power up later
  uint8_t cfg = 0;
  cfg |= NRF_CONFIG_EN_CRC;
  cfg |= NRF_CONFIG_CRCO;
  nrfWriteReg(NRF_REG_CONFIG, cfg);

  // Disable auto-ack for simplicity (suficiente para MIC)
  nrfWriteReg(NRF_REG_EN_AA, 0x00);

  // Enable pipe0
  nrfWriteReg(NRF_REG_EN_RXADDR, 0x01);

  // Address width = 5 bytes
  nrfWriteReg(NRF_REG_SETUP_AW, 0x03);

  // Retries (aunque auto-ack esté off, no molesta)
  nrfWriteReg(NRF_REG_SETUP_RETR, 0x2F);

  // Channel
  nrfWriteReg(NRF_REG_RF_CH, (uint8_t)NRF_CHANNEL);

  // RF_SETUP: 1Mbps, 0dBm (simple)
  // 0x06 suele ser 1Mbps + 0dBm (depende variante). Dejamos 0x06.
  nrfWriteReg(NRF_REG_RF_SETUP, 0x06);

  // Payload width pipe0
  nrfWriteReg(NRF_REG_RX_PW_P0, (uint8_t)NRF_PAYLOAD_LEN);

  // Set addresses
  nrfWriteBuf(NRF_REG_RX_ADDR_P0, NRF_ADDR, 5);
  nrfWriteBuf(NRF_REG_TX_ADDR, NRF_ADDR, 5);

  // Flush FIFOs + clear IRQ
  nrfCmd(NRF_CMD_FLUSH_RX);
  nrfCmd(NRF_CMD_FLUSH_TX);
  nrfClearIrq();

  // Power up
  nrfPowerUp();
}

// -----------------------------
// Public API
// -----------------------------
void funkmodulInitTx(void)
{
  nrfCommonSetup();
  nrfSetRxMode(false);
  ceLow();           // PTX standby
  nrfClearIrq();
}

void funkmodulInitRx(void)
{
  nrfCommonSetup();
  nrfSetRxMode(true);
  nrfClearIrq();
  ceHigh();          // Start listening
  wait_us(150);      // T_stby2a
}

bool funkmodulSendByte(uint8_t b)
{
  // Go to PTX
  ceLow();
  nrfSetRxMode(false);

  // Clear IRQ + flush TX
  nrfClearIrq();
  nrfCmd(NRF_CMD_FLUSH_TX);

  // Write payload
  csnLow();
  spiTransfer(NRF_CMD_W_TX_PAYLOAD);
  spiTransfer(b);
  csnHigh();

  // Pulse CE to transmit (>10us)
  ceHigh();
  wait_us(15);
  ceLow();

  // Wait for TX_DS or MAX_RT (simple polling)
  for (int i = 0; i < 2000; i++) // ~2000*?us (depende wait_us)
  {
    uint8_t st = nrfReadReg(NRF_REG_STATUS);
    if (st & NRF_STATUS_TX_DS)
    {
      nrfWriteReg(NRF_REG_STATUS, NRF_STATUS_TX_DS);
      return true;
    }
    if (st & NRF_STATUS_MAX_RT)
    {
      nrfWriteReg(NRF_REG_STATUS, NRF_STATUS_MAX_RT);
      nrfCmd(NRF_CMD_FLUSH_TX);
      return false;
    }
    wait_us(50);
  }

  // Timeout
  nrfCmd(NRF_CMD_FLUSH_TX);
  return false;
}

bool funkmodulTryReceiveByte(uint8_t *out_b)
{
  if (out_b == NULL)
  {
    return false;
  }

  // Must be in RX mode with CE high (funkmodulInitRx does that)
  uint8_t st = nrfReadReg(NRF_REG_STATUS);
  if ((st & NRF_STATUS_RX_DR) == 0)
  {
    // También podemos mirar FIFO empty
    uint8_t fifo = nrfReadReg(NRF_REG_FIFO_STATUS);
    if (fifo & NRF_FIFO_RX_EMPTY)
    {
      return false;
    }
  }

  // Read payload
  csnLow();
  spiTransfer(NRF_CMD_R_RX_PAYLOAD);
  *out_b = spiTransfer(NRF_CMD_NOP);
  csnHigh();

  // Clear RX_DR
  nrfWriteReg(NRF_REG_STATUS, NRF_STATUS_RX_DR);
  return true;
}

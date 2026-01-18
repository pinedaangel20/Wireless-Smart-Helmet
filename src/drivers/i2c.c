#include "i2c.h"

// * I2C REGISTER BASE ADDRESS POINTER
volatile uint32_t* i2c_reg_base = (volatile uint32_t*)I2C0_BASE;
volatile uint32_t* pads_bank0_base_i2c = (volatile uint32_t*)PADS_BANK0_BASE;

void my_i2c_init()
{
  // Despertar i2c
  volatile uint32_t* resets_clr = (volatile uint32_t*)(RESETS_BASE + 0x3000); // 0x3000 es Atomic Clear
  volatile uint32_t* resets_done = (volatile uint32_t*)(RESETS_BASE + RESETS_RESET_DONE);

  // Escribimos un 1 en el bit 3
  *resets_clr = (1u << RESETS_I2C0_BIT); // pone el bit reset del en 0 para sacarlo del reset

  // Esperamos a que el hardware confirme que el I2C0 está despierto
  while ((*resets_done & (1u << RESETS_I2C0_BIT)) == 0)
  {
     // esperar...
  }

  // configurar pines 4 y 5
  set_gpio_func(SDA_PIN, FUNC_I2C);
  set_gpio_func(SCL_PIN, FUNC_I2C);
  // i2c necesita las resistencias pull up para funcionar
  set_gpio_p(SDA_PIN, PULL_UP);
  set_gpio_p(SCL_PIN, PULL_UP);

  volatile uint32_t* pad_sda = pads_bank0_base_i2c + (1 + SDA_PIN); // offset 0x04 + 4*pin -> indexado uint32
  volatile uint32_t* pad_scl = pads_bank0_base_i2c + (1 + SCL_PIN);
  
  *pad_sda |= (1u << IE_BIT); // IE_BIT es 6 (definido en gpio.h)
  *pad_scl |= (1u << IE_BIT);

  volatile uint32_t* ic_enable_reg = i2c_reg_base + (IC_ENABLE >> 2);
  *ic_enable_reg = 0; // apagarlo antes de configurar master/slave

  // configurar velocidad de fast mode
  *(i2c_reg_base + (IC_FS_SCL_HCNT >> 2)) = 100; // ~800ns High
  *(i2c_reg_base + (IC_FS_SCL_LCNT >> 2)) = 212; // ~1696ns Low

  // configurar modo maestro con el ic_con 
  volatile uint32_t* ic_con_reg = i2c_reg_base + (IC_CON >> 2);
  uint32_t value = 0; // empezamos con un lienzo vacio pq no nos interesa lo que ya esta, dado a que vamos a configurarlo todo
  value |= (1u << IC_MASTER_MODE_BIT); // ponerlo en master mode
  value |= (1u << SPEED_BIT); // 10 para fast mode
  // como comenzamos con un lienzo en blanco, no debemos de poner cosas como 0, pq ya lo son, solo poner cosas como 1
  // 10-bit Address se queda en 0 y por lo mismo usamos 7-bit address para direcciones cortas
  value |= (1u << IC_SLAVE_DISABLE); // desactiva el slava mode
  *ic_con_reg = value;

  volatile uint32_t* ic_tar_reg = i2c_reg_base + (IC_TAR >> 2);
  *ic_tar_reg = OLED_ADDR; // configuramos a la oled screen como slave

  *ic_enable_reg = 1; // volvemos a encenderlo una vez terminamos de configurar
}

void i2c_write_byte(int val, int stop)
{
  volatile uint32_t* ic_status_reg = (i2c_reg_base + (IC_STATUS >> 2));
  volatile uint32_t* ic_data_cmd_reg = (i2c_reg_base + (IC_DATA_CMD >> 2));

  // esparamos a que haya espacio (si escribimos muy rapido en el buffer del hardware se llena)
  while ((*ic_status_reg & (1u << IC_TNF_BIT)) == 0) // mientras el buffer este lleno
  {
    // no hacemos nada solo esperamos a que se vacie
  }

  // Si stop es 1, se activa el bit 9 (STOP bit) del registro de datos
  if (stop) 
  {
    *ic_data_cmd_reg = val | (1u << 9); // dato y stop
  } 
  else 
  {
    *ic_data_cmd_reg = val; // solo dato
  }
}



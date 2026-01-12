#include "i2c.h"

// * I2C REGISTER BASE ADDRESS POINTER
volatile uint32_t* i2c_reg_base = (volatile uint32_t*)I2C0_BASE;

void i2c_init()
{
  // configurar pines 4 y 5
  set_gpio_func(SDA_PIN, FUNC_I2C);
  set_gpio_func(SCL_PIN, FUNC_I2C);
  // i2c necesita las resistencias pull up para funcionar
  set_gpio_p(SDA_PIN, PULL_UP);
  set_gpio_p(SCL_PIN, PULL_UP);

  volatile uint32_t* ic_enable_reg = i2c_reg_base + (IC_ENABLE >> 2);
  *ic_enable_reg = 0; // apagarlo antes de configurar master/slave

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

void i2c_write_byte(int val)
{
  volatile uint32_t* ic_status_reg = (i2c_reg_base + (IC_STATUS >> 2));
  volatile uint32_t* ic_data_cmd_reg = (i2c_reg_base + (IC_DATA_CMD >> 2));

  // esparamos a que haya espacio (si escribimos muy rapido en el buffer del hardware se llena)
  while ((*ic_status_reg & (1u << IC_TNF_BIT)) == 0) // mientras el buffer este lleno
  {
    // no hacemos nada solo esperamos a que se vacie
  }
  *ic_data_cmd_reg = val; // ya es seguro escribir
}



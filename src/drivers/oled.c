#include "oled.h"
#include "stdio.h"

static uint8_t oled_buffer[(SSD1306_HEIGHT * SSD1306_WIDTH) / 8];

static int cursor_x = 0;
static int cursor_y = 0;
static int starting_x = 0;

void oled_init() 
{
   // encender el bus
   my_i2c_init(); 

   // apagar para configurar
   oled_send_command(SSD1306_DISPLAY_OFF);

   // Configura la velocidad de refresco. 
   // 0x80 es el valor sugerido por el fabricante para el mejor balance parpadeo/estabilidad
   oled_send_command(SSD1306_SET_CLOCK_DIV);
   oled_send_command(0x80);

   // Le decimos: "Tienes 64 líneas activas". (0x3F = 63, porque cuenta desde 0).
   oled_send_command(SSD1306_SET_MULTIPLEX);
   oled_send_command(SSD1306_HEIGHT - 1); // 0x3F

   // Le decimos: "Empieza a pintar desde la línea 0 física del cristal".
   oled_send_command(SSD1306_SET_DISPLAY_OFF);
   oled_send_command(0x00);

   // Le decimos: "Mapea la RAM línea 0 a la parte superior".
   oled_send_command(SSD1306_SET_START_LINE | 0x00);

   // Sin esto, la pantalla NO enciende. Habilita el elevador de voltaje interno.
   oled_send_command(SSD1306_CHARGE_PUMP);
   oled_send_command(0x14); // Bit 2 = 1 (Enable)

   // 0x00 = Horizontal Addressing Mode.
   // Significa que al mandar datos, el cursor avanza a la derecha y luego baja solo.
   oled_send_command(SSD1306_MEMORY_MODE);
   oled_send_command(0x00);

   // Estos dos comandos giran la pantalla 180 grados para que coincida con la placa PCB.
   oled_send_command(SSD1306_SEG_REMAP_OP); // Espejo Horizontal
   oled_send_command(SSD1306_COM_SCAN_INV); // Espejo Vertical

   // 0x12 es la configuración física de los electrodos en el panel de 128x64.
   // (Si fuera 128x32, usaríamos 0x02).
   oled_send_command(SSD1306_SET_COMPINS);
   oled_send_command(0x12);

   // 0xCF es un brillo alto. 0x01 (min) - 0xFF (max).
   oled_send_command(SSD1306_SET_CONTRAST);
   oled_send_command(0xCF);

   // Ajustes eléctricos finos para la estabilidad de los píxeles.
   // valores por defecto del datasheet.
   oled_send_command(SSD1306_SET_PRECHARGE);
   oled_send_command(0xF1);
   oled_send_command(SSD1306_SET_VCOM_DESEL);
   oled_send_command(0x40);

   // A4: "Muestra lo que hay en la RAM" (No fuerces encendido total).
   oled_send_command(SSD1306_DISPLAY_RAM); 
   // A6: "Normal" (1=Blanco, 0=Negro). Si usas A7, inviertes colores.
   oled_send_command(0xA6);

   // encender
   oled_send_command(SSD1306_DISPLAY_ON);
}

void oled_clear()
{
   int size = (SSD1306_HEIGHT * SSD1306_WIDTH) / 8;

   // empieza en la columna 0 y fila 0 hasta las ultimas
   oled_send_command(SSD1306_COLUMN_ADDR);
   oled_send_command(0x00);
   oled_send_command(SSD1306_WIDTH - 1);
   oled_send_command(SSD1306_PAGE_ADDR);
   oled_send_command(0x00);
   oled_send_command((SSD1306_HEIGHT / 8) - 1);

   // reescribimos cada parte de la memoria con 0 para limpiar
   for (int i = 0; i < size; i++)
   {
      // no hay que indexar pq el comando MEMORY_MODE mueve el cursor interno a la derecho luego de dibujar en el pixel actual
      oled_send_data(0x00);
   }
}

void oled_render()
{
  // empieza en la columna 0 y fila 0 hasta las ultimas
  oled_send_command(SSD1306_COLUMN_ADDR);
  oled_send_command(0x00);
  oled_send_command(SSD1306_WIDTH - 1);
  oled_send_command(SSD1306_PAGE_ADDR);
  oled_send_command(0x00);
  oled_send_command((SSD1306_HEIGHT / 8) - 1);

  int size = (SSD1306_HEIGHT * SSD1306_WIDTH) / 8;
   
   for (int i = 0; i < size; i++)
   {
      oled_send_data(oled_buffer[i]); 
   }
}

void oled_send_command(int cmd)
{
  i2c_write_byte(SSD1306_CONTROL_CMD, 0); // enviar byte de control para comando
  i2c_write_byte(cmd, 1); // enviar comando real
}

void oled_send_data(int byte)
{
  i2c_write_byte(SSD1306_CONTROL_DATA, 0); // enviar byte de control para datos graficos
  i2c_write_byte(byte, 1); // envia byte de pixel
}

void oled_set_pixel(int x, int y, int color) 
{
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT || x < 0 || y < 0) 
  {
    return;
  }

  int index = x + (y / 8) * SSD1306_WIDTH;
  int bit = y % 8;

  if (color) 
  {
    oled_buffer[index] |= (1 << bit);
  } 
  else 
  {
    oled_buffer[index] &= ~(1 << bit);
  }
}

void oled_draw_char(char c)
{
  // empieza en la columna 0 y fila 0 hasta las ultimas
  oled_send_command(SSD1306_COLUMN_ADDR);
  oled_send_command(cursor_x);
  oled_send_command(SSD1306_WIDTH - 1);
  oled_send_command(SSD1306_PAGE_ADDR);
  oled_send_command(cursor_y);
  oled_send_command((SSD1306_HEIGHT / 8) - 1);

  int start_index = 0; 
  int char_ascii = (int)c;
  int stride = 5;

  int index = stride * (char_ascii - start_index);

  for (int i = 0; i < stride; i++)
  {
    oled_send_data(font[index++]);
  }

  cursor_x += stride + 1; // espacio entre letras
  if (cursor_x >= SSD1306_WIDTH)
  {
    cursor_x = starting_x;
    cursor_y ++;
  }
  
  
}

void oled_set_cursor(int x, int y)
{
  cursor_x = x;
  cursor_y = y;
  starting_x = cursor_x;
}

void oled_print(const char *string)
{
// mientras no se acabe el string
  while (*string) 
  {
    oled_draw_char(*string); // dibuja letra actual
    string++; // avanzamos el puntero a la siguiente letra
  }
}

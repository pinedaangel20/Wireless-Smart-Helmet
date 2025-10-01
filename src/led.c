#include <hardware/gpio.h>
#include <pico/time.h>

#include "led.h"

#define LED_PIN 25

void blink_led(void)
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
}




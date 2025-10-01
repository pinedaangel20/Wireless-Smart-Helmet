#include <stdio.h>
#include <pico/stdio.h>

#include "led.h"

// You can modify this file for local testing, but it will be overridden by the testsystem
int main()
{
    stdio_init_all();

    blink_led();
}

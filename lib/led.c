#include "pico/stdlib.h"

void led_init(uint8_t pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}

void blink(uint8_t pin) {
    gpio_put(pin, true);
    sleep_ms(250);
    gpio_put(pin, false);
    sleep_ms(250);
}
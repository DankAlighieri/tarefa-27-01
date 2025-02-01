#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "lib/matriz_leds.h"
#include "lib/button.h"
#include "lib/led.h"

#define LED_R 13
#define MATRIX_PIN 7
#define BTN_PIN_A 5
#define BTN_PIN_B 6

extern Matriz_leds_config* numeros[];
static volatile uint32_t last_time_a = 0;
static volatile uint32_t last_time_b = 0;
static volatile uint8_t counter = 0;
PIO pio;
uint sm;

static void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
    stdio_init_all();
    button_init(BTN_PIN_A);
    button_init(BTN_PIN_B);
    led_init(LED_R);
    pio = pio0;
    sm = configurar_matriz(pio, MATRIX_PIN);  
    // Starting at zero
    //imprimir_desenho(*numeros[0], pio, sm);

    gpio_set_irq_enabled_with_callback(BTN_PIN_A, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);

    while (true) {
        imprimir_desenho(*numeros[10], pio, sm);
        //blink(LED_R);
    }
}

static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(gpio == BTN_PIN_A && current_time - last_time_a> 400000) {
        counter = (counter + 1) % 10;
        last_time_a = current_time;
    }
    else if(gpio == BTN_PIN_B && current_time - last_time_b > 400000) {
        counter = (counter == 0) ?  9 : counter - 1;
        last_time_b = current_time;
    }
    imprimir_desenho(*numeros[counter],pio, sm);
}
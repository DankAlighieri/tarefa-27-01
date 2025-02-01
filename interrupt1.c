#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"

// Bibliotecas externas para realizar as manipulações dos periféricos
#include "lib/matriz_leds.h"
#include "lib/button.h"
#include "lib/led.h"

#define LED_R 13
#define MATRIX_PIN 7
#define BTN_PIN_A 5
#define BTN_PIN_B 6
#define JOYSTICK_BTN_PIN 22

// Arquivo externo que contém os frames de cada número e um extra para limpar a matriz
extern Matriz_leds_config* numeros[];

// Variáveis voláteis para gerenciar o debounce de cada botão
static volatile uint32_t last_time_a = 0;
static volatile uint32_t last_time_b = 0;
static volatile uint32_t last_time_joy = 0;

static volatile bool last_state_joy = false;

// Contador para incrementar/decrementar
static volatile uint8_t counter = 0;
PIO pio;
uint sm;

static void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
    stdio_init_all();
    button_init(BTN_PIN_A);
    button_init(BTN_PIN_B);
    button_init(JOYSTICK_BTN_PIN);
    led_init(LED_R);
    pio = pio0;
    sm = configurar_matriz(pio, MATRIX_PIN);  
    // Starting at zero
    imprimir_desenho(*numeros[0], pio, sm);

    // Inicializando as interrupções para cada um dos botões
    gpio_set_irq_enabled_with_callback(BTN_PIN_A, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BTN_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);

    while (true) {
        // Função para piscar o led 5x por segundo
        blink(LED_R);
    }
}

// Handler para gerenciar as interrupções de cada botão
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Verifica se é o botão A e realiza o debounce
    if(gpio == BTN_PIN_A && current_time - last_time_a> 200000) {
        // Incrementa o contador e realiza o módulo com 10
        // Dessa forma o contador sempre ficará no intervalo (0,9)
        counter = (counter + 1) % 10;
        last_time_a = current_time;
    }
    
    // Verifica se é o botão B e realiza o debounce
    else if(gpio == BTN_PIN_B && current_time - last_time_b > 200000) {
        // Decrementa o contador, caso seja 0, pula para o 9
        // Dessa forma o contador sempre ficará no intervalo (0,9)
        counter = (counter == 0) ?  9 : counter - 1;
        last_time_b = current_time;
    }

    else if (gpio == JOYSTICK_BTN_PIN && current_time - last_time_joy > 200000) {  
        if (!last_state_joy)
            counter = 10;
        else 
            counter = 0;
        last_state_joy = !last_state_joy;
        last_time_joy = current_time;
    }
    
    // Envia o número para a matriz de LEDs
    imprimir_desenho(*numeros[counter],pio, sm);
}
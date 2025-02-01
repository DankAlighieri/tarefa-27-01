# Tarefa interrupção

## Componentes utilizados
- Placa de estudo BitDogLab

## 💻 Firmware

Desenvolvido em C utilizando:

- IDE: Visual Studio Code
- SDK: Raspberry Pi Pico SDK
- Matriz de LEDs: GP7
- Botão A: GP5
- Botão B: GP6
- Botão Joystick: GP22


## 🎯 Funcionamento

### Estado Inicial

- A matriz de LEDs inicia exibindo o número 0
- O LED RGB pisca 5 vezes por segundo emitindo uma luz vermelha

### Fluxo de Operação:

1. Botão A pressionado:
    - É  chamada uma interrupção pelo hardware, o processador mantém o LED piscando, enquanto o contador interno
    é incrementado e o número exibido na matriz de LEDs é atualizado
    - Caso o número anterior ao incremento seja nove, a lógica interna fará com que ele cicle de volta ao zero,
    exibindo-o na matriz e impedindo que o programa não responda ou dê stackOverflow
2. Botão B pressionado:
    - É  chamada uma interrupção pelo hardware, o processador mantém o LED piscando, enquanto o contador interno
    é decrementado e o número exibido na matriz de LEDs é atualizado 
    - Caso o número anterior ao decremento seja 0, a lógica interna fará com que ele cicle de volta ao nove,
    exibindo-o na matriz e impedindo que o programa não responda ou dê stackOverflow
3. Botão do joystick pressionado
    - Caso seja a primeira vez, a matriz de leds será apagada
    - Caso seja a segunda vez, a matriz de leds irá acender no estado inicial (Mostrando o número zero).

### Explicação

As interrupções são gerenciadas pelas IRQs do GPIO, que acionam um callback *gpio_irq_callback*, onde toda a lógica da interrupção
é realizada.

### Vídeo da placa



## 👥 Autoria
**Guilherme Emetério Santos Lima**  
[![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?style=flat&logo=github)](https://github.com/DankAlighieri)
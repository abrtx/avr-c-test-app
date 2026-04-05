#include "gpio.h"
#include <avr/io.h>

// Map real registers
Gpio GPIOB = { &DDRB, &PORTB, &PINB };
Gpio GPIOC = { &DDRC, &PORTC, &PINC };
Gpio GPIOD = { &DDRD, &PORTD, &PIND };

void gpio_set_mode(Gpio *gpio, uint8_t pin, uint8_t mode) {
    if (mode == GPIO_OUTPUT)
        *gpio->ddr |= (1 << pin);
    else
        *gpio->ddr &= ~(1 << pin);
}

void gpio_write(Gpio *gpio, uint8_t pin, uint8_t value) {
    if (value)
        *gpio->port |= (1 << pin);
    else
        *gpio->port &= ~(1 << pin);
}

void gpio_toggle(Gpio *gpio, uint8_t pin) {
    *gpio->port ^= (1 << pin);
}

uint8_t gpio_read(Gpio *gpio, uint8_t pin) {
    return ((*gpio->pin >> pin) & 1);
}

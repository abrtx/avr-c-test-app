#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Generic GPIO structure
typedef struct {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
} Gpio;

// Pin direction
#define GPIO_OUTPUT 1
#define GPIO_INPUT  0

// API
void gpio_set_mode(Gpio *gpio, uint8_t pin, uint8_t mode);
void gpio_write(Gpio *gpio, uint8_t pin, uint8_t value);
void gpio_toggle(Gpio *gpio, uint8_t pin);
uint8_t gpio_read(Gpio *gpio, uint8_t pin);

// Predefined ports (ATmega328P)
extern Gpio GPIOB;
extern Gpio GPIOC;
extern Gpio GPIOD;

#endif

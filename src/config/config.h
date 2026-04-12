#ifndef CONFIG_H
#define CONFIG_H

#include "gpio.h"

// -----------------------------
// LEDs
// -----------------------------
#define LED1_GPIO GPIOB
#define LED1_PIN  2

#define LED2_GPIO GPIOB
#define LED2_PIN  3

#define LED3_GPIO GPIOB
#define LED3_PIN  4

#define LED_COUNT 3

// -----------------------------
// BUTTONS
// -----------------------------
#define BUTTON1_GPIO GPIOD
#define BUTTON1_PIN  2
#define BUTTON1_MODE BUTTON_ACTIVE_LOW

#endif

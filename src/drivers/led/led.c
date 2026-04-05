#include "led.h"

void led_init(Led *led) {
    gpio_set_mode(led->gpio, led->pin, GPIO_OUTPUT);
}

void led_on(Led *led) {
    gpio_write(led->gpio, led->pin, 1);
}

void led_off(Led *led) {
    gpio_write(led->gpio, led->pin, 0);
}

void led_toggle(Led *led) {
    gpio_toggle(led->gpio, led->pin);
}

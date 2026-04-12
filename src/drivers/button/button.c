#include "button.h"
#include "timer.h"

#define DEBOUNCE_MS 50

// -----------------------------
static uint8_t read_raw(Button *btn) {

    uint8_t val = gpio_read(btn->gpio, btn->pin);

    if (btn->mode == BUTTON_ACTIVE_LOW)
        return !val;

    return val;
}

// -----------------------------
void button_init(Button *btn) {

    gpio_set_mode(btn->gpio, btn->pin, GPIO_INPUT);

    // enable pull-up if active low
    if (btn->mode == BUTTON_ACTIVE_LOW) {
        gpio_write(btn->gpio, btn->pin, 1);
    }

    btn->last_state = 0;
    btn->last_time = 0;
    btn->pressed_flag = 0;
}

// -----------------------------
void button_update(Button *btn) {

    uint8_t current = read_raw(btn);
    uint32_t now = timer_now();

    // detect change
    if (current != btn->last_state) {

        if ((now - btn->last_time) > DEBOUNCE_MS) {

            btn->last_state = current;
            btn->last_time = now;

            if (current) {
                btn->pressed_flag = 1;
            }
        }
    }
}

// -----------------------------
uint8_t button_pressed(Button *btn) {

    if (btn->pressed_flag) {
        btn->pressed_flag = 0;
        return 1;
    }

    return 0;
}

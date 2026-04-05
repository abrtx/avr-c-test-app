#ifndef APP_H
#define APP_H

void app_init(void);
void app_loop(void);

// shared timing
extern volatile unsigned long millis;

#endif

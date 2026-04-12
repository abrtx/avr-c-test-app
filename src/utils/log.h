#ifndef LOG_H
#define LOG_H

void log_init(void);
void log_write(const char *msg);

// NEW
void log_fmt(const char *fmt, ...);

#endif

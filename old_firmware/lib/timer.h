#ifndef TIMER_H
#define TIMER_H 

typedef void (*timer_cb)(void);

void start_timer(timer_cb cb);

void stop_timer(void);

#endif /* TIMER_H */

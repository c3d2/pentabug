#ifndef LISTEN_H
#define LISTEN_H 

#include <stdint.h>

void listen_init(void);

void listen_stop(void);

uint16_t listen_measure(void);

#endif /* LISTEN_H */

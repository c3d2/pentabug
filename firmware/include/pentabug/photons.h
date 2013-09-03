#ifndef PHOTONS_H
#define PHOTONS_H 

#include <stdint.h>

void photons_init(void);

void photons_stop(void);

uint8_t photons_measure(void);

#endif /* PHOTONS_H */

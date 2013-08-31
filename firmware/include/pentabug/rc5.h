#ifndef RC5_H
#define RC5_H 

#include <stdint.h>

struct rc5_data {
	uint8_t addr;
	uint8_t code;
};

void rc5_send(struct rc5_data *data);

#endif /* RC5_H */

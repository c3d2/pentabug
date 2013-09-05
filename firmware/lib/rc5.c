#include <pentabug/rc5.h>

#include <pentabug/hal.h>

#define HALF_BIT_TICKS		68

static uint8_t toggle = 0;

static void send_bit(uint8_t bit) {
	ir_set(!bit);
	wait_ticks(HALF_BIT_TICKS);

	ir_set(bit);
	wait_ticks(HALF_BIT_TICKS);
}

static void send_byte_part(uint8_t byte, uint8_t length) {
	int8_t i;

	for(i = length - 1; i >= 0; --i) {
		send_bit(byte & (1 << i));
	}
}

void rc5_send(struct rc5_data *data) {
	// start bit

	send_bit(1);

	// field bit

	send_bit(!(data->code & (1 << 6)));

	// control bit

	send_bit(toggle);
	toggle = !toggle;

	// address

	send_byte_part(data->addr, 5);

	// command

	send_byte_part(data->code, 6);

	// done

	ir_off();
}


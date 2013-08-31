#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/rc5.h>

static void run(void) {
	if(button_clicked(LEFT)) {
		led_on(RIGHT);

		struct rc5_data data = { 0, 12 };

		uint8_t i;
		for(i = 0; i < 32; ++i) {
			/*led_inv(LEFT);*/

			data.addr = i;

			uint8_t n;
			for(n = 0; n < 128; ++n) {
				data.code = n;
				rc5_send(&data);

				wait_ms(120);
			}
		}

		led_off(RIGHT);
	}
}

REG(run);

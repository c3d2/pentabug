#include <pentabug/lifecycle.h>

jmp_buf app_jmp_buf;
volatile uint8_t app_should_stop = 0;
volatile int8_t app_direction = 1;

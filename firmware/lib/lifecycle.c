#include <pentabug/lifecycle.h>

jmp_buf app_jmp_buf;
volatile uint8_t should_stop;

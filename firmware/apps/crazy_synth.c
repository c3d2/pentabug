#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"
#include "../lib/synth.h"

/**
 * do crazy synthesizer mode
 *
 */
static void crazy_synth(void)
{
	/* initialisation required */
	if (mode_uninitialized) {
		mode_uninitialized = false;
		synth_init();
	}

	/*deinialisation required */
	if (mode_last_tick) {
		synth_deinit();
	}

	return;
}

REGISTER(crazy_synth)


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>

#include "synth.h"


static void fill_buffer(void* userdata, Uint8* stream, int len) {
	for(int i = 0; i < len / 2; i++) ((uint16_t*)stream)[i] = synth_mix() * 100;
}

SDL_AudioSpec spec = { 8000000 / (3 * 64), AUDIO_U16SYS, 1, 0, 1024, };

int main(int argc ,char** argv) {

	synth_init();

	spec.callback = &fill_buffer;
	if(SDL_OpenAudio(&spec, &spec) < 0) {
		fprintf(stderr, "ERROR");
		exit(1);
	}
	printf("freq = %d\n", spec.freq);

	SDL_PauseAudio(0);

	getchar();

	return 0;
}

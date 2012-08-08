#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "synth.h"


enum { MIXRATE = 8000000 / (6 * 64) };

static SDL_AudioSpec spec = { MIXRATE, AUDIO_U16SYS, 1, 0, 1024, };

static void fill_buffer(void* userdata, Uint8* stream, int len) {
	for(int i = 0; i < len / 2; i++) {
		uint16_t m = synth_mix();
		assert(m < 0x100);
		((uint16_t*)stream)[i] = m * 50;
	}
}

// this parser is a complete hack
// put i can't care less right now
static int parse_tune(const char* filename) {
	FILE* file = fopen(filename, "r");
	if(!file) return -1;


	char wave_names[256][256];
	memset(wave_names, 0, sizeof(wave_names));
	int wave_counter = 0;


	char inst_names[256][256];
	memset(inst_names, 0, sizeof(inst_names));
	int inst_counter = 0;


	char pattern_names[256][256];
	memset(pattern_names, 0, sizeof(pattern_names));
	int pattern_counter = 0;

	tune_length = 0;


	int state = 0;
	int line_nr = 0;
	char line[256];
	while(fgets(line, 256, file)) {
		line_nr++;

		// skip empty line
		if(line[0] == '\n') continue;

		// comments
		if(line[0] == '#') continue;

		if(state == 0) {
			if(strcmp("[WAVETABLE]\n", line)) return line_nr;
			state = 1;
		}
		else if(state == 1) {
			if(line[0] == '[') {
				if(strcmp("[INSTRUMENTS]\n", line)) return line_nr;
				state = 2;
			}
			else if(!isspace(line[0])) {
				sscanf(line, "%s", wave_names[wave_counter]);
			}
			else if(line[0] == '\t') {
				int a, b;
				if(sscanf(line + 1, "%u %u", &a, &b) != 2) return line_nr;
				wave_table[wave_counter][0] = a;
				wave_table[wave_counter][1] = b;
				wave_counter++;
			}
			else return line_nr;
		}
		else if(state == 2) {
			if(line[0] == '[') {
				if(strcmp("[PATTERNS]\n", line)) return line_nr;
				state = 3;
			}
			else {
				char wave[256];
				int pw, ps, d;
				if(sscanf(line, "%s %u %u %u %s",
					inst_names[inst_counter], &pw, &ps, &d,
					wave) != 5) return line_nr;

				instruments[inst_counter].pulse_width = pw;
				instruments[inst_counter].pulse_sweep = ps;
				instruments[inst_counter].decay = d;
				int i;
				for(i = 0; i < 256; i++)
					if(strcmp(wave_names[i], wave) == 0) break;
				if(i == 256) return line_nr;
				instruments[inst_counter].wave_table_pos = i;
				inst_counter++;
			}
		}
		else if(state == 3) {
			if(line[0] == '[') {
				if(strcmp("[PATTERNTABLE]\n", line)) return line_nr;
				state = 4;
			}
			else {
				if(isspace(line[0])) return line_nr;
				sscanf(line, "%s", pattern_names[pattern_counter]);
				for(int i = 0; i < pattern_length && fgets(line, 256, file); i++) {
					line_nr++;
					if(line[0] != '\t') return line_nr;
					char note[256];
					char inst[256];
					int m = sscanf(line + 1, "%s %s", note, inst);
					if(m == 0) return line_nr;
					char* s = "ccddeffggaab";
					char* p = strchr(s, line[1]);
					if(p) {
						patterns[pattern_counter][i][0] = (p - s) +
								(note[1] == '#') + (note[2] - '0') * 12;
						int j;
						for(j = 0; j < 256; j++)
							if(strcmp(inst_names[j], inst) == 0) break;
						if(j == 256) return line_nr;
						patterns[pattern_counter][i][1] = j;

					}
					else if(m == 1) {
						if(strcmp("---", note) == 0)
							patterns[pattern_counter][i][0] = 0xff;
						else if(strcmp(".", note) == 0)
							patterns[pattern_counter][i][0] = 0;
						else return line_nr;

					}
					else return line_nr;

				}
				pattern_counter++;
			}



		}
		else {
			char* p = line;
			char pat[256];
			for(int i = 0; i < channel_count; i ++) {
				sscanf(p, "%s", pat);
				int j;
				for(j = 0; j < 256; j++)
					if(strcmp(pattern_names[j], pat) == 0) break;
				if(j == 256) return line_nr;
				pattern_table[tune_length][i] = j;
				while(*p && !isspace(*p)) p++;
				while(*p && isspace(*p)) p++;
			}
			tune_length++;


		}
	}
	if(state =! 4) return line_nr;


	fclose(file);
	return 0;
}




int main(int argc, char** argv) {
	if(argc != 2) {
		printf("usage: %s tunefile\n", argv[0]);
		return 0;
	}

	int error = parse_tune(argv[1]);
	if(error != 0) {
		fprintf(stderr, "%d: parsing error\n", error);
		return 1;
	}

	spec.callback = &fill_buffer;
	if(SDL_OpenAudio(&spec, &spec) < 0) {
		fprintf(stderr, "ERROR");
		exit(1);
	}

	SDL_PauseAudio(0);
	puts("playing...");

	getchar();

	return 0;
}

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

static int wave_counter;
static int inst_counter;
static int pattern_counter;


static int parse_tune(const char* filename) {
	FILE* file = fopen(filename, "r");
	if(!file) return -1;

	wave_counter = 0;
	inst_counter = 0;
	pattern_counter = 0;
	tune_length = 0;

	char wave_names[256][256];
	char inst_names[256][256];
	char pattern_names[256][256];
	memset(wave_names, 0, sizeof(wave_names));
	memset(inst_names, 0, sizeof(inst_names));
	memset(pattern_names, 0, sizeof(pattern_names));


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


static int export_tune(const char* filename) {
	FILE* file = fopen(filename, "w");
	if(!file) return -1;

	fprintf(file, "uint8_t wave_table[][2] = {\n");
	for(int i = 0; i < wave_counter; i++) {
		fprintf(file, "\t{ %d, %d },\n", wave_table[i][0], wave_table[i][1]);
	}
	fprintf(file, "};\n\n");

	fprintf(file, "synth_instrument_t instruments[] = {\n");
	for(int i = 0; i < inst_counter; i++) {
		fprintf(file, "\t{ %d, %d, %d, %d },\n",
			instruments[i].pulse_width,
			instruments[i].pulse_sweep,
			instruments[i].decay,
			instruments[i].wave_table_pos);
	}
	fprintf(file, "};\n\n");

	fprintf(file, "uint8_t patterns[][pattern_length][2] = {\n");
	for(int i = 0; i < pattern_counter; i++) {
		fprintf(file, "\t{\n");
		for(int j = 0; j < pattern_length; j++) {
			fprintf(file, "\t\t{ %d, %d },\n", patterns[i][j][0], patterns[i][j][1]);
		}
		fprintf(file, "\t},\n");
	}
	fprintf(file, "};\n\n");

	fprintf(file, "uint8_t pattern_table[][channel_count] = {\n");
	for(int i = 0; i < tune_length; i++) {
		fprintf(file, "\t{ ");
		for(int j = 0; j < channel_count; j++) {
			fprintf(file, "%d ", pattern_table[i][j]);
		}
		fprintf(file, "},\n");
	}
	fprintf(file, "};\n\n");
	fprintf(file, "enum { pattern_table_length = %d };\n", tune_length);
	fclose(file);

	int size = wave_counter * 2 +
		inst_counter * sizeof(synth_instrument_t) +
		pattern_counter * pattern_length * 2 +
		tune_length * channel_count;
	printf("tune size: %d bytes\n", size);

	return 0;
}


int main(int argc, char** argv) {
	if(argc != 2) {
		printf("usage: %s tunefile\n", argv[0]);
		return 0;
	}

	puts("parsing...");
	int error = parse_tune(argv[1]);
	if(error != 0) {
		fprintf(stderr, "%d: parsing error\n", error);
		return 1;
	}
	puts("exporting...");
	export_tune("tune.h");


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

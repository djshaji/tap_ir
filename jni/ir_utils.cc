/*
    Copyright (C) 2011 Tom Szilagyi

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "ir.h"
#include "ir_utils.h"

/* Hash function to obtain key to IR file path.
 * This is the djb2 algorithm taken from:
 *   http://www.cse.yorku.ca/~oz/hash.html
 */
uint64_t fhash(char *str) {
        uint64_t hash = 5381;
        int c;
	
        while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
        return hash;
}

uint64_t fhash_from_ports(float *port0, float *port1, float *port2) {
	uint64_t val0 = ((uint64_t)*port0) & 0xffff;
	uint64_t val1 = ((uint64_t)*port1) & 0xffffff;
	uint64_t val2 = ((uint64_t)*port2) & 0xffffff;
	return (val0 << 48) + (val1 << 24) + val2;
}

void ports_from_fhash(uint64_t fhash, float *port0, float *port1, float *port2) {
	*port0 = (float)((fhash >> 48) & 0xffff);
	*port1 = (float)((fhash >> 24) & 0xffffff);
	*port2 = (float)(fhash & 0xffffff);
}

int filename_filter(const char * file) {
	if (!file) { return 0; }
	if (strlen(file) < 5) { return 0; }
	const char * ext = file + strlen(file)-4;
	if (strcmp(ext, ".wav") == 0) {	return 1; }
	if (strcmp(ext, ".WAV") == 0) {	return 1; }
	if (strcmp(ext, ".aiff") == 0) { return 1; }
	if (strcmp(ext, ".AIFF") == 0) { return 1; }
	if (strcmp(ext, ".au") == 0) { return 1; }
	if (strcmp(ext, ".AU") == 0) { return 1; }
	if (strcmp(ext, ".flac") == 0) { return 1; }
	if (strcmp(ext, ".FLAC") == 0) { return 1; }
	if (strcmp(ext, ".ogg") == 0) { return 1; }
	if (strcmp(ext, ".OGG") == 0) { return 1; }
	return 0;
}

int dirname_filter(const char * file) {
	if (!file) { return 0; }
	if (strlen(file) < 1) { return 0; }
	if (file[0] == '.') { return 0; }
	return 1;
}


#define TAU 0.25
/* attack & envelope curves where t = 0..1 */
#define ATTACK_FN(t) (exp(((t) - 1.0) / TAU))
#define ENVELOPE_FN(t) (exp(-1.0 * (t) / TAU))

void compute_envelope(float ** samples, int nchan, int nfram,
		      int attack_time_s, float attack_pc,
		      float env_pc, float length_pc) {
	float gain;

	if (attack_time_s > nfram) {
		attack_time_s = nfram;
	}

	for (int j = 0; j < attack_time_s; j++) {
		gain = attack_pc / 100.0 +
			(100.0 - attack_pc) / 100.0 *
			ATTACK_FN((double)j / attack_time_s);
		for (int i = 0; i < nchan; i++) {
			samples[i][j] *= gain;
		}
	}

	int length_s = length_pc / 100.0 * (nfram - attack_time_s);
	for (int j = attack_time_s, k = 0; j < attack_time_s + length_s; j++, k++) {
		gain = env_pc / 100.0 +
			(100.0 - env_pc) / 100.0 *
			ENVELOPE_FN((double)k / length_s);
		for (int i = 0; i < nchan; i++) {
			samples[i][j] *= gain;
		}
	}

	for (int j = attack_time_s + length_s; j < nfram; j++) {
		for (int i = 0; i < nchan; i++) {
			samples[i][j] = 0.0f;
		}
	}
}

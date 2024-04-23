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

#ifndef _IR_UTILS_H
#define _IR_UTILS_H

#include <inttypes.h>
#include "ir.h"

#define IR_SAVE_FILE    ".ir_save"
#define GROUP_FHASH     "file-hashes"
#define GROUP_BOOKMARKS "bookmarks"


uint64_t fhash(char *str);
uint64_t fhash_from_ports(float *port0, float *port1, float *port2);
void ports_from_fhash(uint64_t fhash, float *port0, float *port1, float *port2);

void compute_envelope(float ** samples, int nchan, int nfram,
		      int attack_time_s, float attack_pc,
		      float env_pc, float length_pc);


#endif /* _IR_UTILS_H */

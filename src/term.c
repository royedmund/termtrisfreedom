/*
Termtris - a tetris game for ANSI/VT100 terminals
Copyright (C) 2019-2023  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void vt52_init(void);
void ansi_init(void);
void adm3_init(void);
void freedom100_init(void);
#if defined(MSDOS) || defined(__COM__)
void pcbios_init(void);
#endif

char *termenv;
int term_type;

void (*term_reset)(void);
void (*term_clearscr)(void);
void (*term_setcursor)(int row, int col);
void (*term_cursor)(int show);
void (*term_setcolor)(int fg, int bg);
void (*term_ibmchar)(unsigned char c, unsigned char attr);



void term_init(void)
{
	char *env;
	int vtnum;

	if((env = getenv("TERM"))) {
		int len = strlen(env);
		if((termenv = calloc(1, len < 8 ? 8 : len + 1))) {
			strcpy(termenv, env);

			env = termenv;
			while(*env) {
				*env = tolower(*env);
				env++;
			}
		}
	}

#if defined(MSDOS) || defined(__COM__)
	if(!termenv) {
		pcbios_init();
		return;
	}
#else
	if(!termenv) {
		goto ansi;	/* if TERM is unset, assume ANSI */
	}
#endif

	if(termenv[0] == 'v' && termenv[1] == 't') {
		vtnum = atoi(termenv + 2);

		if(vtnum >= 52 && vtnum < 100) {
			vt52_init();
		} else {
			ansi_init();
		}
		return;
	}

	if(memcmp(termenv, "adm3", 4) == 0) {
		adm3_init();
		return;
	}
	if(memcmp(termenv, "freedom100", 10) == 0) {
		freedom100_init();
		return;
	}

ansi:
	/* for unknown terminals, try to use ANSI */
	ansi_init();
}

void term_putstr(const char *s, unsigned char attr)
{
	while(*s) {
		term_ibmchar(*s++, attr);
	}
}

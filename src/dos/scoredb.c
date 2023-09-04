
/*
Termtris - a tetris game for ANSI/VT220 terminals
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
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "scoredb.h"
#include "ansi.h"

static int name_dialog(char *buf);

extern const char *progpath;

#define MAX_NAME	36
struct score_entry {
	char user[MAX_NAME];
	long score, lines, level;
};

#define NUM_SCORES	10
static struct score_entry scores[NUM_SCORES];


int save_score(long score, long lines, long level)
{
	int i, rest;
	FILE *fp;
	long offs;

	for(i=0; i<NUM_SCORES; i++) {
		if(!*scores[i].user || score > scores[i].score) {
			rest = NUM_SCORES - i - 1;
			if(rest > 0) {
				memmove(scores + i + 1, scores + i, rest * sizeof *scores);
			}
			break;
		}
	}

	if(i == NUM_SCORES) return 0;

	/* get name */
	name_dialog(scores[i].user);
	if(!*scores[i].user) {
		strcpy(scores[i].user, "dosuser");
	}
	scores[i].score = score;
	scores[i].lines = lines;
	scores[i].level = level;


	/* save the new scores table */
	offs = (long)scores - 256;

	if(!(fp = fopen(progpath, "r+b"))) {
		return -1;
	}
	fseek(fp, offs, SEEK_SET);
	fwrite(scores, 1, sizeof scores, fp);
	fclose(fp);
	return 0;
}

int print_scores(int num)
{
	int i;
	struct score_entry *sc = scores;

	for(i=0; i<NUM_SCORES; i++) {
		if(!*sc->user) break;
		
		printf("%2d. %s - %ld pts  (%ld lines)\n", i + 1, sc->user, sc->score,
				sc->lines);
		sc++;
	}
	return 0;
}

#define DLG_W	(MAX_NAME + 2)
#define DLG_X	((80 - DLG_W) / 2)
#define ATTR	7
#define VALID(x)	(isalnum(x) || (x) == ' ' || (x) == '_' || (x) == '-')

static int name_dialog(char *buf)
{
	int i, j, key, cur;
	char *s;

	ansi_setcursor(8, DLG_X);
	ansi_ibmchar(G_UL_CORNER, ATTR);
	for(i=1; i<DLG_W-1; i++) {
		ansi_ibmchar(G_HLINE, ATTR);
	}
	ansi_ibmchar(G_UR_CORNER, ATTR);

	ansi_setcursor(9, DLG_X);
	ansi_ibmchar(G_VLINE, ATTR);
	for(i=1; i<DLG_W-1; i++) {
		ansi_ibmchar(' ', 0x70);
	}
	ansi_ibmchar(G_VLINE, ATTR);
	ansi_ibmchar(G_CHECKER, 0x70);

	ansi_setcursor(10, DLG_X);
	ansi_ibmchar(G_LL_CORNER, ATTR);
	for(i=1; i<DLG_W-1; i++) {
		ansi_ibmchar(G_HLINE, ATTR);
	}
	ansi_ibmchar(G_LR_CORNER, ATTR);
	ansi_ibmchar(G_CHECKER, 0x70);

	ansi_setcursor(11, DLG_X + 1);
	for(i=0; i<DLG_W; i++) {
		ansi_ibmchar(G_CHECKER, 0x70);
	}

	ansi_setcursor(8, DLG_X + 1);
	ansi_putstr(" High score! ", ATTR);
	ansi_setcursor(9, DLG_X + 1);
	ansi_cursor(1);

	cur = 0;
	for(;;) {
		key = getch();
		switch(key) {
		case 27:
			ansi_cursor(0);
			return -1;

		case '\r':
		case '\n':
			return 0;

		case '\b':
			if(cur > 0) {
				buf[--cur] = 0;
			}
			break;

		default:
			if(VALID(key) && cur < MAX_NAME - 1) {
				buf[cur++] = key;
				buf[cur] = 0;
			}
		}
		
		s = buf;
		ansi_setcursor(9, DLG_X + 1);
		for(i=0; i<DLG_W-2; i++) {
			if(*s) {
				ansi_ibmchar(*s++, 0x70);
			} else {
				ansi_ibmchar(' ', 0x70);
			}
		}
		ansi_setcursor(9, DLG_X + cur + 1);
	}

	ansi_cursor(0);
	return 0;
}
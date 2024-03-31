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
#include "game.h"
#include "term.h"

void freedom100_reset(void);
void freedom100_clearscr(void);
void freedom100_setcursor(int row, int col);
void freedom100_cursor(int show);
void freedom100_setcolor(int fg, int bg);
void freedom100_ibmchar(unsigned char c, unsigned char attr);

void freedom100_init(void)
{
	/* TODO: Optimise for FREEDOM100
	 */
	onlyascii = 1;

	term_type = TERM_FREEDOM100;
	term_reset = freedom100_reset;
	term_clearscr = freedom100_clearscr;
	term_setcursor = freedom100_setcursor;
	term_cursor = freedom100_cursor;
	term_setcolor = freedom100_setcolor;
	term_ibmchar = freedom100_ibmchar;
}

void freedom100_reset(void)
{
	freedom100_clearscr();
}

void freedom100_clearscr(void)
{
	fputc(032, stdout);	/* SUB: clearscr */
}

void freedom100_setcursor(int row, int col)
{
	if(!(row | col)) {
		fputc(036, stdout);	/* RS: home */
	} else {
		printf("\033=%c%c", row + 32, col + 32);
	}
}

void freedom100_cursor(int show)
{
}

void freedom100_setcolor(int fg, int bg)
{
}

void freedom100_ibmchar(unsigned char c, unsigned char attr)
{
	fputc(c, stdout);
}

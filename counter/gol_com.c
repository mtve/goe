#include <stdio.h>
#include <stdlib.h>

#include "gol.h"

void
or_die (int i, char *str) {
	if (i)
		return;
	printf ("%s\n", str);
	exit (0);
}

void
field_dmp (field_t *f) {
	int		x, y;

	for (y = 0; y < Y; y++) {
		for (x = 0; x < X; x++)
			printf ("%c",
			    f->c[y][x] == C_DED ? '.' :
			    f->c[y][x] == C_LIV ? 'X' :
			    f->c[y][x] == C_ANY ? '#' :
			    f->c[y][x] == C_TRY ? '?' : '!');
		printf ("\n");
	}
	printf ("\n");
}

void
field_cpy (field_t *dst, field_t *src) {
	int		x, y;

	for (y = 0; y < Y; y++)
	for (x = 0; x < X; x++)
		dst->c[y][x] = src->c[y][x];
}

int
field_eq (field_t *dst, field_t *src) {
	int		x, y;

	for (y = 0; y < Y; y++)
	for (x = 0; x < X; x++)
		if (dst->c[y][x] != src->c[y][x])
			return 0;
	return 1;
}

void
field_set (field_t *f, int val) {
	int		x, y;

	for (y = 0; y < Y; y++)
	for (x = 0; x < X; x++)
		f->c[y][x] = val;
}

void
box_xy (int xa, int xb, int ya, int yb, char *xx, char *yy) {
	int			g, x, y;

	/* boxes */
	g = 0;
	for (y = ya; y < xb || y < yb; y++) {
		for (x = xa; y < yb && x < xb && x < y; x++, g++)
			yy[g] = y, xx[g] = x;
		for (x = y < yb ? y : yb - 1; x >= xa && y < xb; x--, g++)
			yy[g] = x, xx[g] = y;
	}
	or_die (g == (yb - ya) * (xb - xa), "wtf gen");

	if (0) {
		for (g = 0; g < (yb - ya) * (xb - xa); g++)
			printf ("%d:%d ", yy[g], xx[g]);
		exit (1);
	}

}

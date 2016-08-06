#include <stdio.h>
#include <stdlib.h>

#include "gol.h"

#define LLIM	10000000

static char	arr[] = {
 -1,  -1,
0
};

static void
f_set (field_t *f, int x, int j) {
	f->c[2][x] = j &  1 ? C_LIV : C_DED;
	f->c[3][x] = j &  2 ? C_LIV : C_DED;
	f->c[4][x] = j &  4 ? C_LIV : C_DED;
	f->c[5][x] = j &  8 ? C_LIV : C_DED;
	f->c[6][x] = j & 16 ? C_LIV : C_DED;
}

int
main (void) {
	int			x1, x, j, jbest;
	long			l, lmin;
	static field_t		f;

	field_set (&f, C_ANY);

	for (x = 2; arr[x]; x++)
		f_set (&f, x, arr[x]);

	x1 = x < 7 ? 2 : x - 5;
	for (; x < X - 2; x++) {
		par_init (x1, x);
		jbest = 0;
		lmin = -1;
		for (j = 31; j >= 0; j--) {
			f_set (&f, x, j);
			l = par_cnt (&f, 0, 0);
			if (!l)
				return 1;
			if (!jbest || l < lmin)
				jbest = j, lmin = l;
			printf ("    %02x %ld, %02x\n", j, l, jbest);
			fflush (stdout);
		}
		if (lmin > LLIM)
			x1++;
		f_set (&f, x, jbest);
		printf ("%d = %02x (%ld), %d\n", x, jbest, lmin, x1);
		fflush (stdout);
	}
	return 0;
}

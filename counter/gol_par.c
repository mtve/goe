#include "gol.h"

static field_t		par;
static field_t		chi;
static char		nei[Y][X][2];

#define PAR_BOX	((X-2)*(Y-2))
static int 		par_x[PAR_BOX];
static int 		par_y[PAR_BOX];
static int		par_size;

static int
tst (int x, int y) {
	switch (chi.c[y][x]) {
	case 0:
		/* alive & >=2 nei & <=3 nei -> alive */
		if (par.c[y][x] == C_LIV && nei[y][x][C_LIV] >= 2 && nei[y][x][C_DED] >= 5)
			return 0;
		/* dead & =3 nei -> alive */
		if (par.c[y][x] == C_DED && nei[y][x][C_LIV] == 3 && nei[y][x][C_DED] == 5)
			return 0;
		break;
	case 1:
		/* >3 nei -> dead */
		if (nei[y][x][C_LIV] > 3)
			return 0;
		/* <2 nei (>6 dead nei) -> dead */
		if (nei[y][x][C_DED] > 6)
			return 0;
		/* dead & <3 nei (>5 dead nei) -> dead */
		if (par.c[y][x] == C_DED && nei[y][x][C_DED] > 5)
			return 0;
		break;
	}
	return 1;
}

static void
inc (int x, int y, int v) {
	nei[y-1][x-1][v]++; nei[y-1][x][v]++; nei[y-1][x+1][v]++; 
	nei[y  ][x-1][v]++;                   nei[y  ][x+1][v]++; 
	nei[y+1][x-1][v]++; nei[y+1][x][v]++; nei[y+1][x+1][v]++; 
}

static void
dec (int x, int y, int v) {
	nei[y-1][x-1][v]--; nei[y-1][x][v]--; nei[y-1][x+1][v]--; 
	nei[y  ][x-1][v]--;                   nei[y  ][x+1][v]--; 
	nei[y+1][x-1][v]--; nei[y+1][x][v]--; nei[y+1][x+1][v]--; 
}

static int
try (int x, int y) {
	return tst (x-1,y-1) && tst (x,y-1) && tst (x+1,y-1)
	    && tst (x-1,y  ) && tst (x,y  ) && tst (x+1,y  )
	    && tst (x-1,y+1) && tst (x,y+1) && tst (x+1,y+1);
}

long
par_cnt (field_t *f, long lim, long *pbt) {
	int		i, x, y;
	long		cnt, bt;

	bt = 0;
	field_cpy (&chi, f);

	for (y = 0; y < Y; y++)
	for (x = 0; x < X; x++) {
		nei[y][x][0] = nei[y][x][1] = 0;
		par.c[y][x] = C_ANY;
	}

	for (y = 2; y < Y - 2; y++)
	for (x = 2; x < X - 2; x++) {
		if (chi.c[y][x] != C_ANY)
			par.c[y-1][x-1] = par.c[y-1][x] = par.c[y-1][x+1] =
			par.c[y  ][x-1] = par.c[y  ][x] = par.c[y  ][x+1] =
			par.c[y+1][x-1] = par.c[y+1][x] = par.c[y+1][x+1] =
			     C_TRY;
	}

	cnt = 0;
	/* unrolled recursive descent */
	for (i = 0; ; i++) {
		if (i == par_size) {
			cnt++;
			if (cnt == lim)
				break;
			goto UP_ANY;
		}
		x = par_x[i], y = par_y[i];
		if (par.c[y][x] != C_TRY)
			continue;

		par.c[y][x] = C_DED;
		inc (x, y, C_DED);
		if (try (x, y))
			continue;
TRY_LIV:	dec (x, y, C_DED);

		par.c[y][x] = C_LIV;
		inc (x, y, C_LIV);
		if (try (x, y))
			continue;
UP_TRY:		dec (x, y, C_LIV);

		par.c[y][x] = C_TRY;
		bt++;

UP_ANY:		if (--i < 0)
			break;
		x = par_x[i], y = par_y[i];
		switch (par.c[y][x]) {
		case C_DED:	goto TRY_LIV;
		case C_LIV:	goto UP_TRY;
		case C_ANY:	goto UP_ANY;
		default:	or_die (0, "wtf");
		}
	}
	if (pbt)
		*pbt = bt;
	return cnt;
}

void
par_init (int x1, int x2) {
	int		i, x, y;

	i = 0;
	par_size = (x2 - x1 + 3) * (Y - 2);
	for (x = x1 - 1; x <= x2 + 1; x++)
	for (y = 1; y <= Y - 2; y++)
		par_x[i] = x, par_y[i] = y, i++;
	or_die (i == par_size, "wtf1");
}

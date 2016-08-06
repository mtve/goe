#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sat_cms.h"

#define MA		45
#define L4_MAX		(MA*MA)
#define L0_MAX		((MA+4)*(MA+4))

static int		L4_X;
#define L4_Y		(L4_X)
#define L3_X		(L4_X+2)
#define L3_Y		(L4_Y+2)
#define L2_X		(L4_X+4)
#define L2_Y		(L4_Y+4)
#define L1_X		(L4_X+6)
#define L1_Y		(L4_Y+6)
#define L0_X		(L4_X+8)
#define L0_Y		(L4_Y+8)
#define L4_BOX		(L4_X*L4_Y)
#define L3_BOX		(L3_X*L3_Y)
#define L2_BOX		(L2_X*L2_Y)
#define L1_BOX		(L1_X*L1_Y)
#define L0_BOX		(L0_X*L0_Y)

typedef int		(*C_t) (int, int);

static SAT		*sat_b;

static int		l4_len;
static char		chi[L4_MAX];

static int		xx[L0_MAX];
static int		yy[L0_MAX];

static char		start[] =
"11111110110110101010101110111010101101001010111101010011011110110101101110011011110110101010001011111101101110011111010101110110101001101110010101101111111011010100110101111101111110101101100110010111101111110111011010001100110010011001101110000011110010001001001111000111111000110011100010110011011100001110110011100100001111110101011111111001011100000000100111111001000001100101010100111110111001111010010101010100010110001000111111100011110110011111110101000010011000110010000101111111011100100100010010010110100010001100101101100101111100100010010011111100110011110000011011101100100101110010000110010001010011000010100111001010111100100001100110010011101011001100010100011001010001101011001101110101000010010110010000110001111000001011";

void
or_die (int i, const char *str) {
	if (i)
		return;
	printf ("%s\n", str);
	exit (0);
}

static int
L0_C (int y, int x) {
	return y * L0_X + x + 1;
}

static int
L1_C (int y, int x) {
	return L0_BOX + (y - 1) * L1_X + (x - 1) + 1;
}

static int
L2_C (int y, int x) {
	return L0_BOX + L1_BOX + (y - 2) * L2_X + (x - 2) + 1;
}

static int
L3_C (int y, int x) {
	return L0_BOX + L1_BOX + L2_BOX + (y - 3) * L3_X + (x - 3) + 1;
}

static int
L4_C (int y, int x) {
	return L0_BOX + L1_BOX + L2_BOX + L3_BOX + (y - 4) * L4_X + (x - 4) + 1;
}

static void
spiral (int C) {
	int		i, j, k, x, y;
	int		xy[MA+4][MA+4];

	yy[0] = C; xx[0] = C;
	i = 1;
	for (j = 1; j <= C; j++) {
		for (k = C + j - 1; k >= C - j; k--)
			yy[i] = C + j, xx[i] = k, i++;
		for (k = C + j - 1; k >= C - j; k--)
			yy[i] = k, xx[i] = C - j, i++;
		for (k = C - j + 1; k <= C + j; k++)
			yy[i] = C - j, xx[i] = k, i++;
		for (k = C - j + 1; k <= C + j; k++)
			yy[i] = k, xx[i] = C + j, i++;
	}
	or_die (i == L0_BOX, "spiral");
	for (y = 0; y < L0_Y; y++)
	for (x = 0; x < L0_X; x++)
		xy[y][x] = -1;
	for (i = 0; i < L0_BOX; i++) {
		or_die (xx[i] >= 0, "x0");
		or_die (xx[i] <  L0_X, "xm");
		or_die (yy[i] >= 0, "y0");
		or_die (yy[i] <  L0_Y, "ym");
		xy[yy[i]][xx[i]] = i;
	}
	if (0) {
		printf ("spiral %d:\n", C);
		for (y = 0; y < L0_Y; y++) {
			for (x = 0; x < L0_X; x++)
				printf ("%4d", xy[y][x]);
			printf ("\n");
		}
	}
	for (y = 0; y < L0_Y; y++)
	for (x = 0; x < L0_X; x++)
		or_die (xy[y][x] != -1, "-1");
}



static void
add_cell (SAT *sat, int x, int y, C_t PAR, C_t CHI) {
#include "gen.inc"
}

static void
construct (void) {
	int		x, y, i;

	for (y = 1; y < L1_Y + 1; y++)
	for (x = 1; x < L1_X + 1; x++)
		add_cell (sat_b, x, y, L0_C, L1_C);
	for (y = 2; y < L2_Y + 2; y++)
	for (x = 2; x < L2_X + 2; x++)
		add_cell (sat_b, x, y, L1_C, L2_C);
	for (y = 3; y < L3_Y + 3; y++)
	for (x = 3; x < L3_X + 3; x++)
		add_cell (sat_b, x, y, L2_C, L3_C);
	for (y = 4; y < L4_Y + 4; y++)
	for (x = 4; x < L4_X + 4; x++)
		add_cell (sat_b, x, y, L3_C, L4_C);

	for (i = 0; i < l4_len - 1; i++) {
		sat_b->add_lit (L4_C (yy[i], xx[i]) * (chi[i] ? 1 : -1));
		sat_b->add_lit (0);
	}
}

static void
dmp_child (void) {
	char		c[MA+4][MA+4];
	int		x, y, i;

	for (y = 4; y < L4_Y + 4; y++)
	for (x = 4; x < L4_X + 4; x++)
		c[y][x] = '?';
	for (i = 0; i < l4_len; i++)
		c[yy[i]][xx[i]] = chi[i] ? 'X' : '.';
	for (y = 4; y < L4_Y + 4; y++) {
		for (x = 4; x < L4_X + 4; x++)
			printf ("%c", c[y][x]);
		printf ("\n");
	}
	printf ("\n");
	fflush (stdout);
}

static int
isgoe (void) {
	int		i;
	int		x, y;
	SAT		sat;

	for (y = 1; y < L1_Y + 1; y++)
	for (x = 1; x < L1_X + 1; x++)
		add_cell (&sat, x, y, L0_C, L1_C);

	for (i = 0; i < l4_len; i++) {
		sat.add_lit (L1_C (yy[i], xx[i]) * (chi[i] ? 1 : -1));
		sat.add_lit (0);
	}
	return sat.solve ();
}

static int
isgoe2 (void) {
	int		i;
	int		x, y;
	SAT		sat;

	for (y = 1; y < L1_Y + 1; y++)
	for (x = 1; x < L1_X + 1; x++)
		add_cell (&sat, x, y, L0_C, L1_C);
	for (y = 2; y < L2_Y + 2; y++)
	for (x = 2; x < L2_X + 2; x++)
		add_cell (&sat, x, y, L1_C, L2_C);

	for (i = 0; i < l4_len; i++) {
		sat.add_lit (L2_C (yy[i], xx[i]) * (chi[i] ? 1 : -1));
		sat.add_lit (0);
	}
	return sat.solve ();
}

static int
isgoe3 (void) {
	int		i;
	int		x, y;
	SAT		sat;

	for (y = 1; y < L1_Y + 1; y++)
	for (x = 1; x < L1_X + 1; x++)
		add_cell (&sat, x, y, L0_C, L1_C);
	for (y = 2; y < L2_Y + 2; y++)
	for (x = 2; x < L2_X + 2; x++)
		add_cell (&sat, x, y, L1_C, L2_C);
	for (y = 3; y < L3_Y + 3; y++)
	for (x = 3; x < L3_X + 3; x++)
		add_cell (&sat, x, y, L2_C, L3_C);

	for (i = 0; i < l4_len; i++) {
		sat.add_lit (L3_C (yy[i], xx[i]) * (chi[i] ? 1 : -1));
		sat.add_lit (0);
	}
	return sat.solve ();
}

static int
issat (char *par, int l0_len) {
	int			i;

	i = l4_len - 1;
	sat_b->assume (L4_C (yy[i], xx[i]) * (chi[i] ? 1 : -1));

	for (i = 0; i < l0_len; i++)
		sat_b->assume (L0_C (yy[i], xx[i]) * (par[i] ? 1 : -1));

	time_t		t;

	time (&t);
	fprintf (stderr, "l4=%d, l0=%d, %s\n", l4_len, l0_len, ctime (&t));
	return sat_b->solve ();
}

static void
go () {
	int			l0_len, c, p, s[2], d[2];
	char			par[2][2][L0_MAX];

	while (l4_len < L4_MAX) {
		l4_len++;
		for (L4_X = 1; L4_X < MA; L4_X += 2)
			if (L4_X * L4_X >= l4_len)
				break;
		spiral ((L4_X - 1) / 2 + 4);
		
		sat_b = new SAT ();
		construct ();
		for (l0_len = 1; l0_len <= L0_BOX; l0_len++) {
			for (c = 0; c < 2; c++) {
				chi[l4_len-1] = c;
				for (p = 0; p < 2; p++) {
					par[c][p][l0_len-1] = p;
					s[p] = issat (par[c][p], l0_len);
					if (!s[p])
						par[c][p][l0_len-1] = !p;
				}
				d[c] = s[1] + s[0];
				if (!d[c] && l0_len == 1) {
					if (!isgoe ()) {
						dmp_child ();
						if (!isgoe2 ()) {
							if (!isgoe3 ()) {
								printf ("goe3!\n");
								exit (1);
							} else {
								printf ("goe2!\n");
							}
						} else {
							printf ("goe!\n");
						}
						fflush (stdout);
					}
					chi[l4_len-1] = !c;
					goto NEXT;
				}
			}
			if (0) {
				for (c = 0; c < 4; c++) {
					printf ("chi=%d par=%d: ", c/2, c%2);
					for (p = 0; p < l0_len; p++)
						printf ("%d", par[c/2][c%2][p]);
					printf ("\n");
				}	
				fflush (stdout);
			}
			if (d[0] != d[1]) {
				chi[l4_len-1] = d[0] < d[1] ? 0 : 1;
				break;
			}			
		}
NEXT:		printf ("\n>>> box %d l4_len %d depth %d/%d\n", L4_X, l4_len,
		    l0_len, L0_BOX);
		for (c = 0; c < l4_len; c++)
			printf ("%d", chi[c]);
		printf ("\n\n");
		dmp_child ();
		delete sat_b;
	}
}

int
main (void) {
	int		i;

	l4_len = strlen (start);
	for (i = 0; i < l4_len; i++)
		chi[i] = start[i] & 1;

	go ();

	return 0;
}

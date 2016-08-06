#ifndef _GOL_H
#define _GOL_H

#define Y	(5+2+2)
#define X	(1000+2+2)

#define C_DED	0
#define	C_LIV	1
#define C_ANY	2
#define C_TRY	3

typedef	struct {
	char		c[Y][X];
} field_t;

/* gol_com.c */
void			or_die (int i, char *str);
void			field_dmp (field_t *f);
void			field_set (field_t *f, int val);
int			field_eq  (field_t *a, field_t *b);
void			field_cpy (field_t *dst, field_t *src);
void			box_xy (int xa, int xb, int ya, int yb, char *xx, char *yy);

/* gol_par.c */
long			par_cnt (field_t *f, long sollim, long *btcnt);
void			par_init (int x1, int x2);

#endif

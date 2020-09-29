#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

FLOAT filter(struct filter *f, FLOAT x)
{
	unsigned int i;
	FLOAT div;

	i = f->dim <= 1 ? 0 : f->cnt++ % f->dim;
	div = f->dim <= 1 ? 1 : f->cnt < f->dim ? f->cnt : f->dim;

	f->f -= f->x[i];
	f->x[i] = x;
	f->f += f->x[i];

	return f->f / div;
}

/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static int _rand(void)
{
	static int init = 1;

	if (init) {
		init = 0;
		srand(0);
	}
	return rand();
}

float frand(float min, float max)
{
	float d = (float)_rand();

	d = (d * (max - min)) / (float)RAND_MAX;
	return min + d;
}

/* derived from 'Numerical Recipes in C' 2nd Ed. */
float gaussian_noise(void)
{
	static int n2_cached = 0;
	static float n2 = 0.0f;
	float x, y, r, d, n1;

	if (n2_cached) {
		n2_cached = 0;
		return n2;
	}

	do {
		x = frand(-1.0f, 1.0f);
		y = frand(-1.0f, 1.0f);
		r = x * x + y * y;
	} while (r == 0.0f || r > 1.0f);

	d = sqrt(-2.0 * log(r) / r) / 5.0f;
	n1 = x * d;
	n2 = y * d;

	n2_cached = 1;
	return n1;
}

//#define TEST
#ifdef TEST
static int _compar(const void *o1, const void *o2)
{
	float f1 = *(float *)o1, f2 = *(float *)o2;

	if (f1 == f2)
		return 0;
	return f1 < f2 ? -1 : 1;
}

#define CNT 3000000
int main(int argc, char *argv[])
{
	float *f = malloc(sizeof(*f) * CNT);
	int i;

	for (i = 0; i < CNT; i++)
		f[i] = gaussian_noise();

	qsort(f, CNT, sizeof(*f), _compar);

	//for (i = 0; i < CNT; i++)
	for (i = 0; i < 10; i++)
		printf("%5.4f\n", f[i]);
	printf("\n");
	for (i = CNT - 10; i < CNT; i++)
		printf("%5.4f\n", f[i]);

	return 0;
}
#endif

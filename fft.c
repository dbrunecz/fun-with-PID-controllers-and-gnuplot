/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

static int validate_power_of_2_size(unsigned int size)
{
	int m;

	for (m = 0; m < 32; m++)
		if (size & BIT(m))
			break;

	if (m == 32 || (size & ~(BIT(m + 1) - 1)))
		return -1;

	return m;
}

static inline unsigned int get_bit_reversed_val(unsigned int width,
						unsigned int n)
{
	unsigned int i, v = 0;

	for (i = 0; i < width; i++)
		v |= n & BIT(i) ? BIT(width - 1 - i) : 0;
	return v;
}

static inline void swap_vals(FLOAT * array, unsigned int n1, unsigned int n2)
{
	FLOAT f;

	f = array[n1];
	array[n1] = array[n2];
	array[n2] = f;
}

void fft(unsigned int cnt, FLOAT * tre, FLOAT * tim)
{
	unsigned int i, j, k, idx1, idx2;
	unsigned int subdftsz, subdftcnt;
	FLOAT re, im;
	FLOAT ur, ui;
	FLOAT tr, ti;
	int m;

	m = validate_power_of_2_size(cnt);
	if (m < 0) {
		DBG();
		return;
	}

	for (i = 1; i < cnt / 2; i++) {
		k = get_bit_reversed_val(m, i);
		swap_vals(tre, i, k);
		swap_vals(tim, i, k);
	}

	for (i = 1; i <= m; i++) {
		subdftsz = BIT(i);
		subdftcnt = BIT(m - i);

		ur = 1.0f;
		ui = 0.0f;

		re = COS(PI / BIT(i - 1));
		im = -SIN(PI / BIT(i - 1));

		for (j = 0; j < subdftcnt; j++) {
			for (k = 0; k < subdftsz; k++) {
				idx1 = (j * subdftsz + k);	// % subdftsz;
				idx2 = (idx1 + subdftsz / 2) % subdftsz;

				if (idx1 >= cnt || idx2 >= cnt) {
					DBG();
					exit(-1);
				}
				tr = tre[idx1] * ur - tim[idx1] * ui;
				ti = tre[idx1] * ui + tim[idx1] * ur;

				tre[idx1] = tre[idx2] - tr;
				tim[idx1] = tim[idx2] - ti;

				tre[idx2] = tre[idx2] + tr;
				tim[idx2] = tim[idx2] - ti;
			}
			tr = ur;
			ur = tr * re - ui * im;
			ui = tr * im + ui * re;
		}
	}
}

//void ifft(unsigned int cnt, FLOAT * tre, FLOAT * tim, FLOAT * fre, FLOAT * fim)
void ifft(unsigned int cnt, FLOAT *re, FLOAT *im)
{
	int m, i;

	m = validate_power_of_2_size(cnt);
	if (m < 0) {
		DBG();
		return;
	}

	for (i = 0; i < cnt; i++)
		im[i] *= -1.0;

	fft(cnt, re, im);

	for (i = 0; i < cnt; i++) {
		re[i] /= cnt;
		im[i] /= cnt;
		im[i] *= -1.0;
	}
}

#define TEST
#ifdef TEST
int main(int argc, char *argv[])
{
	unsigned int i, samplerate, duration, samplecount;
	FLOAT *re, *im;
	FLOAT t, dt;

	samplerate = 256;
	duration = 8;

	samplecount = samplerate * duration;

	re = malloc(sizeof(*re) * samplecount);
	im = malloc(sizeof(*im) * samplecount);

	if (!re || !im) {
		DBG();
		return -1;
	}

	dt = 1.0f / (FLOAT) samplerate;

	for (i = 0, t = 0.0f; i < samplecount; i++, t += dt) {
		re[i] = 4.0f;
		//re[i] += 3.0f * SIN(2 * PI * 0.25f * t + 0.0f);
		re[i] += 2.0f * SIN(2 * PI * 0.5f * t + 0.0f);
		//re[i] += 4.0f * COS(2 * PI * 1.0f * t + 0.0f);
		//printf("%4.3f % 4.3f\n", t, tre[i]);
	}
	memset(im, 0, sizeof(*im) * samplecount);

#if 0
	for (i = 0; i < samplecount; i++)
		printf("%2.4f %2.4f\n", i * dt, re[i]);
#else
	fft(samplecount, re, im);
	for (i = 0; i < samplecount / 2; i++)
		printf("%f %f\n", i * dt,
		       0.5f * POW(re[i] * re[i] + im[i] * im[i], 0.5f));
#endif
	//ifft(samplecount, re, im);

	//for (i = 0; i < samplecount / 2; i++)
	//    printf("%f %f\n", (FLOAT)i / (FLOAT)duration,
	//            0.5f * POW(re[i] * re[i] + im[i] * im[i], 0.5f));

	free(re);
	free(im);

	return 0;
}
#endif

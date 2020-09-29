#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void idft(FLOAT * in, unsigned int cnt, FLOAT * re, FLOAT * im)
{
	FLOAT a = 2 * PI / (FLOAT) cnt;
	unsigned int i, j;

	memset(in, 0, sizeof(*in) * cnt);

	for (i = 0; i < cnt / 2 + 1; i++) {
		re[i] /= cnt / 2;
		im[i] /= cnt / 2;
	}
	re[0] /= 2;
	re[cnt / 2] /= 2;

	for (i = 0; i < cnt / 2 + 1; i++) {
		for (j = 0; j < cnt; j++) {
			in[j] += re[i] * COS(a * i * j);
			in[j] -= im[i] * SIN(a * i * j);
		}
	}
}

void dft(FLOAT * in, unsigned int cnt, FLOAT * re, FLOAT * im)
{
	FLOAT a = 2 * PI / (FLOAT) cnt;
	unsigned int i, j;

	memset(re, 0, sizeof(*re) * cnt / 2);
	memset(im, 0, sizeof(*im) * cnt / 2);

	for (i = 0; i < cnt / 2 + 1; i++) {
		for (j = 0; j < cnt; j++) {
			re[i] += in[j] * COS(a * i * j);
			im[i] -= in[j] * SIN(a * i * j);
		}
	}
}

void complex_idft(unsigned int cnt, FLOAT * tre, FLOAT * tim,
		  FLOAT * fre, FLOAT * fim)
{
}

void complex_dft(unsigned int cnt, FLOAT * tre, FLOAT * tim,
		 FLOAT * fre, FLOAT * fim)
{
	FLOAT a = 2 * PI / (FLOAT) cnt;
	unsigned int f, t;
	FLOAT re, im;

	memset(fre, 0, sizeof(*fre) * cnt);
	memset(fim, 0, sizeof(*fim) * cnt);

	for (f = 0; f < cnt; f++) {
		for (t = 0; t < cnt; t++) {
			re = COS(a * f * t);
			im = -1.0 * SIN(a * f * t);
			fre[f] += re * tre[t] - im * tim[t];
			fim[f] += im * tre[t] + re * tim[t];
		}
	}
}

#define TEST
#ifdef TEST
#if 0
int main(int argc, char *argv[])
{
	unsigned int samplerate, duration, samplecount;
	FLOAT *in;
	FLOAT *out;
	unsigned int i;
	FLOAT *re, *im;
	FLOAT t, dt;

	samplerate = 200;
	duration = 5;

	samplecount = samplerate * duration;

	in = malloc(sizeof(*in) * samplecount);
	out = malloc(sizeof(*out) * (samplecount + 2));

	if (!in || !out) {
		DBG();
		return -1;
	}

	dt = 1.0f / (FLOAT) samplerate;

	for (i = 0, t = 0.0f; i < samplecount; i++, t += dt) {
		//in[i] = 4.0f;
		in[i] = 7.0f * SIN(2 * PI * 0.3f * t + 0.0f);
		in[i] += 1.0f * SIN(2 * PI * 5.0f * t + 0.0f);
		//in[i] += 5.0f * COS(2 * PI * 70.0f * t + 0.0f);
		//printf("%4.3f % 4.3f\n", t, in[i]);
	}

	re = &out[0];
	im = &out[samplecount / 2 + 1];

	dft(in, samplecount, re, im);
	idft(in, samplecount, re, im);

	for (i = 0, t = 0.0f; i < samplecount; i++, t += dt)
		printf("%4.3f % 4.3f\n", t, in[i]);

	//for (i = 0; i < samplecount / 2 + 1; i++)
	//    printf("%f %f\n", (FLOAT)i / (FLOAT)duration, 
	//            0.5f * POW(re[i] * re[i] + im[i] * im[i], 0.5f));

	free(in);
	free(out);

	return 0;
}
#else
int main(int argc, char *argv[])
{
	unsigned int i, samplerate, duration, samplecount;
	FLOAT *tre, *tim;
	FLOAT *fre, *fim;
	FLOAT t, dt;

	samplerate = 200;
	duration = 5;

	samplecount = samplerate * duration;

	tre = malloc(sizeof(*tre) * samplecount);
	tim = malloc(sizeof(*tim) * samplecount);
	fre = malloc(sizeof(*fre) * samplecount);
	fim = malloc(sizeof(*fim) * samplecount);

	if (!tre || !tim || !fre || !fim) {
		DBG();
		return -1;
	}

	dt = 1.0f / (FLOAT) samplerate;

	for (i = 0, t = 0.0f; i < samplecount; i++, t += dt) {
		tre[i] = 4.0f;
		tre[i] += 7.0f * SIN(2 * PI * 30.3f * t + 0.0f);
		tre[i] += 1.0f * SIN(2 * PI * 5.0f * t + 0.0f);
		tre[i] += 5.0f * COS(2 * PI * 70.0f * t + 0.0f);
		//printf("%4.3f % 4.3f\n", t, tre[i]);
	}
	memset(tim, 0, sizeof(*tim) * samplecount);

	complex_dft(samplecount, tre, tim, fre, fim);

	//for (i = 0, t = 0.0f; i < samplecount; i++, t += dt)
	//    printf("%4.3f % 4.3f\n", i, tre[i]);

	for (i = 0; i < samplecount / 2; i++)
		printf("%f %f\n", (FLOAT) i / (FLOAT) duration,
		       0.5f * POW(fre[i] * fre[i] + fim[i] * fim[i], 0.5f));

	free(tre);
	free(tim);
	free(fre);
	free(fim);

	return 0;
}
#endif
#endif

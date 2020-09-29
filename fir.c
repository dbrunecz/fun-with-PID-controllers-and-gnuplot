#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

unsigned int samplerate, duration;

unsigned int samplecount;
FLOAT *in;
FLOAT *out;

//unsigned int firsize;
//FLOAT *fir;

int main(int argc, char *argv[])
{
	unsigned int i;
	FLOAT *re, *im;
	FLOAT t, dt;

	samplerate = 200;
	duration = 5;

	samplecount = samplerate * duration;

	in = malloc(sizeof(*in) * samplecount);
	out = malloc(sizeof(*out) * (samplecount + 2));
	//fir = malloc(sizeof(*fir) * firsize);

	if (!in || !out /* || !fir */ ) {
		DBG();
		return -1;
	}

	dt = 1.0f / (FLOAT) samplerate;

	for (i = 0, t = 0.0f; i < samplecount; i++, t += dt) {
		//in[i] = 4.0f;
		in[i] = 7.0f * SIN(2 * PI * 0.3f * t + 0.0f);
		in[i] += 1.0f * SIN(2 * PI * 5.0f * t + 0.0f);
		//in[i] += 5.0f * COS(2 * PI * 70.0f * t + 0.0f);
		printf("%4.3f % 4.3f\n", t, in[i]);
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
	//free(fir);

	return 0;
}

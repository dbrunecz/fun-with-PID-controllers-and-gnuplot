/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */

#include <math.h>

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
void four1(float data[], unsigned long nn, int isign)
{
	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;

	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2) {
		if (j > i) {
			SWAP(data[j + 0], data[i + 0]);
			SWAP(data[j + 1], data[i + 1]);
		}
		m = n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}

	mmax = 2;
	while (n > mmax) {
		istep = mmax << 1;
		theta = isign * (6.28318530717959 / mmax);

		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);

		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;
				tempr = wr * data[j + 0] - wi * data[j + 1];
				tempi = wr * data[j + 1] + wi * data[j + 0];

				data[j + 0] = data[i + 0] - tempr;
				data[j + 1] = data[i + 1] - tempi;
				data[i + 0] += tempr;
				data[i + 1] += tempi;
			}
			wr = (wtemp = wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
int main(int argc, char *argv[])
{
	unsigned int i, samplerate, duration, samplecount;
	float *f;
	float t, dt;

	samplerate = 256;
	duration = 8;

	samplecount = samplerate * duration;

	f = malloc(sizeof(*f) * (samplecount + 1) * 2);

	if (!f) {
		DBG();
		return -1;
	}

	dt = 1.0f / (float)samplerate;

	for (i = 0, t = 0.0f; i < samplecount; i += 2, t += dt) {
		f[i + 1] = 0.0f;
		f[i] = 4.0f;
		f[i] += 7.0f * SIN(2 * PI * 30.3f * t + 0.0f);
		f[i] += 1.0f * SIN(2 * PI * 5.0f * t + 0.0f);
		f[i] += 5.0f * COS(2 * PI * 70.0f * t + 0.0f);
		//printf("%4.3f % 4.3f\n", t, tre[i]);
	}

	four1(f, samplecount, 1);

	for (i = 0; i < samplecount; i += 2)
		printf("%f %f\n", (FLOAT) (i / 2) / (FLOAT) duration,
		       0.5f * POW(f[i] * f[i] + f[i + 1] * f[i + 1], 0.5f));
	//for (i = 0; i < samplecount / 2; i++)
	//    printf("%f %f\n", (FLOAT)i / (FLOAT)duration,
	//            0.5f * POW(re[i] * re[i] + im[i] * im[i], 0.5f));

	free(f);

	return 0;
}

/* Copyright (C) 2020 David Brunecz. Subject to GPL 2.0 */


#ifndef __COMMON_H__
#define __COMMON_H__
/******************************************************************************/

static inline char * _bn(char *s)
{
	char *p;

	for (p = s; *p; p++)
		;
	for (p--; p > s && *p != '/'; p--)
		;
	if (p > s && *p == '/')
		p++;
	return *p == '\0' ? s : p;
}

#define DBGFMT              "%s:%d %s() "
#define DBGARGS             _bn(__FILE__), __LINE__, __func__
#define DBG(fmt, a...)      fprintf(stderr, DBGFMT fmt "\n", DBGARGS, ## a)

#define DIM(x)      (sizeof(x)/sizeof((x)[0]))

#define BIT(x)      (1 << (x))

#define MIN(x, y)   (((x) < (y)) ? (x) : (y))
#define MAX(x, y)   (((x) > (y)) ? (x) : (y))

#define PI          ((FLOAT)3.14159265)

#if 1
#define FLOAT double
#define ABS   fabs
#define POW   pow
#define COS   cos
#define SIN   sin
#else
#define FLOAT float
#define ABS   fabsf
#define POW   powf
#define COS   cosf
#define SIN   sinf
#endif

float gaussian_noise(void);

FLOAT transform(FLOAT x1, FLOAT x2, FLOAT x, FLOAT y1, FLOAT y2);

struct pid {
	FLOAT sp;
	FLOAT err;
	FLOAT kp;
	FLOAT ki;
	FLOAT kd;
	FLOAT i;
	FLOAT isat;
	FLOAT d;
	FLOAT qu;
};

FLOAT update_pid(struct pid *pid, FLOAT state);

struct filter {
	FLOAT x[4];
	FLOAT f;
	unsigned int dim;
	unsigned int cnt;
};

FLOAT filter(struct filter *f, FLOAT x);

//void ifft(unsigned int cnt, FLOAT *tre, FLOAT *tim, FLOAT *fre, FLOAT *fim);
void ifft(unsigned int cnt, FLOAT *re, FLOAT *im);
void fft(unsigned int cnt, FLOAT *tre, FLOAT *tim);

void idft(FLOAT *in, unsigned int cnt, FLOAT *re, FLOAT *im);
void dft(FLOAT *in, unsigned int cnt, FLOAT *re, FLOAT *im);

/******************************************************************************/
#endif

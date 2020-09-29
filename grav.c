#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0

v = d / t a = v / t v2 = v1 + a * dt x2 = x1 + v * dt = 0.5 * a * dt
#endif
#define FLOAT double
void apply_acceleration(FLOAT * x, FLOAT * v, FLOAT a, FLOAT dt)
{
	*x = *x + (*v * dt) + (0.5f * a * dt * dt);
	*v = *v + (a * dt);
}

int main(int argc, char *argv[])
{
	FLOAT x = 100.0f, v = 0.0f, a = -9.8f;
	FLOAT t, dt = 0.001f;

	for (t = 0.0f; x > 0.0f; t += dt) {
		apply_acceleration(&x, &v, a, dt);
		printf("%5.4f %4.3f %4.3f\n", t, x, v);
	}

	return 0;
}

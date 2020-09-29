#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int sign(FLOAT f)
{
	return f < 0.0f ? -1 : 1;
}

void step_state(FLOAT * x, FLOAT * v, FLOAT a, FLOAT dt)
{
	*x = *x + (*v * dt) + (0.5f * a * dt * dt);
	*v = *v + (a * dt);
}

FLOAT transform(FLOAT x1, FLOAT x2, FLOAT x, FLOAT y1, FLOAT y2)
{
	x = MIN(MAX(x, x1), x2);
	x = (x - x1) / (x2 - x1);
	return y1 + x * (y2 - y1);
}

FLOAT func1(FLOAT t)
{
	if (t < 10 || t > 25)
		return 0.0f;
	return 0.1f * (7.5f - ABS(t - 17.5f)) * SIN(0.1f * 2 * PI * t + 0.0f);
}

FLOAT func3(FLOAT t)
{
	return 1.0f * COS(0.8f * 2 * PI * t + 0.0f);
}

FLOAT func2(FLOAT t)
{
	FLOAT x;

	x = -0.4f * POW(t - 62.5f, 2) + 2.0f;
	return MAX(0.0f, x);
}

FLOAT update_sp(FLOAT t)
{
	struct range {
		FLOAT t1;
		FLOAT t2;
		FLOAT sp;
		 FLOAT(*func) (FLOAT);
	} rs[] = {
#if 0
		{
		1.0f, 2.0f, 1.0f, NULL}, {
		2.0f, 3.0f, -1.0f, NULL}, {
		3.5f, 5.5f, 0.0f, func3},
#else
		{
		2.0f, 5.0f, 3.0f, NULL}, {
		9.0f, 26.0f, 0.0f, func1}, {
		30.0f, 35.0f, -2.0f, NULL}, {
		40.0f, 50.0f, 4.5f, NULL}, {
		55.0f, 70.0f, 0.0f, func2}, {
		75.0f, 80.0f, -0.5f, NULL}, {
		80.0f, 81.0f, 1.0f, NULL}, {
		82.0f, 83.0f, -1.0f, NULL}, {
		83.5f, 85.5f, 0.0f, func3},
#endif
	};
	int i;

	for (i = 0; i < DIM(rs); i++)
		if (t >= rs[i].t1 && t <= rs[i].t2)
			return rs[i].func ? rs[i].func(t) : rs[i].sp;
	return 0.0f;
}

FLOAT update_pid(struct pid * pid, FLOAT state)
{
	FLOAT delta;

	pid->err = state - pid->sp;

	if (pid->qu > 0.0f)
		pid->err *= ABS(pid->err) * pid->qu;

	pid->i += pid->err;

	if (pid->isat > 0.0f)
		pid->i = MIN(pid->i, pid->isat);

	delta = pid->err - pid->d;
	pid->d = pid->err;

	return pid->kp * pid->err + pid->ki * pid->i + pid->kd * delta;
}

struct state {
	FLOAT x;
	FLOAT v;
};

struct system {
	struct state state;
	struct pid pid;
	struct filter fv;

	FLOAT prevu;
	FLOAT du;
	FLOAT un;
	FLOAT vn;

	FLOAT cw;

	FLOAT kg;
	FLOAT pv;

	unsigned int spins;
	unsigned int spinmod;
};

FLOAT a = -9.8f;

FLOAT t1 = 0.0f;
#if 0
FLOAT t2 = 6.0f;
#else
FLOAT t2 = 90.0f;
#endif
FLOAT dt = 0.1f;		// affects, kp ...

// any filter( == delay) == overshoot, compensate with kd ?

// faster sample rate always better

void init_system(int id, struct system *s)
{
	memset(&s->pid, 0, sizeof(s->pid));

	//s->pid.kp = 6.0f; // 10 Hz, no du, no kg, no noise
	s->pid.kp = 3.0f;
	//s->pid.kp = 8.0f + id * 1.0f;

	//s->pid.ki = 0.000f;//0.01f;
	s->pid.ki = 0.01f + id * 0.01f;

	//s->pid.kd = 0.0f;//0.45f;
	s->pid.kd = 4.0f;	//0.45f;
	//s->pid.kd = 2.0f + id * 2.5f;

	//s->pid.isat = 2.0f;
	s->pid.isat = 0.0f;
	//s->pid.isat = 10.0f - id * 2.4f;

	s->pid.qu = 0;		//id * 0.25f;

	s->state.x = 100.0f;
	s->state.v = 0.0f;

	s->cw = 3.0f;		//4 + 2.0f * id;

	s->vn = 0.8f;		//1.2f;
	//s->vn = 0.0f + id * 0.4f;

	s->un = 0.2f;		//1.2f;

	//s->du = 0.000001f + id * 0.000002f;
	s->du = 0.0f;

	s->prevu = 10.0f;

	s->spins = 0;
	//s->spinmod = 0;
	s->spinmod = 0;		//1 + id;

	memset(&s->fv, 0, sizeof(s->fv));
	//s->fv.dim = 6 * (id + 1);

	s->kg = 0.2f;
	//s->kg = 0.0f + id * 0.20f;
}

int main(int argc, char *argv[])
{
	struct system systems[4], *s;
	unsigned int i, j;
	FLOAT u, ctrl, mv, kg;
	FLOAT t;

	for (i = 0; i < DIM(systems); i++)
		init_system(i, &systems[i]);

	for (j = 0, t = t1; t < t2; t += dt, j++) {
		printf("%5.4f % 4.3f ", t, update_sp(t));

		for (i = 0; i < DIM(systems); i++) {
			s = &systems[i];

			if (s->spinmod <= 1 || !(j % s->spinmod)) {
				s->spins++;
				s->pid.sp = update_sp(t);

				mv = s->state.v + gaussian_noise() * s->vn;

				//mv = filter(&s->fv, mv);

				if (s->kg > 0) {
					kg = s->kg + transform(0.3f, 1.0f,
							       s->pid.err, 0.0f,
							       1.0f - s->kg);
					mv = kg * mv + (1 - kg) * s->pv;
					s->pv = mv;
				}

				ctrl = update_pid(&s->pid, mv);

				u = transform(-s->cw, s->cw, ctrl, 20.0f, 0.0f);
				u = MIN(MAX(0.0f, u), 20.0f);

				if (s->du > 0.0f) {
					if (ABS(u - s->prevu) >
					    (s->du * dt * s->spinmod))
						u = u <
						    s->prevu ? u -
						    s->du * dt *
						    s->spinmod : u +
						    s->du * dt * s->spinmod;
					s->prevu = u;
				}
			} else {
				u = s->prevu;
			}

			u = u + gaussian_noise() * s->un;
			u = MIN(MAX(0.0f, u), 20.0f);

			step_state(&s->state.x, &s->state.v, a + u, dt);

			printf("% 4.3f ", mv);	//s->state.v);
		}

		printf("\n");
	}

	for (i = 0; i < DIM(systems); i++)
		fprintf(stderr, "%d %d\n", i, systems[i].spins);

	return 0;
}

#ifndef MYUTILS_H
#define MYUTILS_H

#include "mbed.h"

// TODO put all project wide defines in here
#define TELEMETRY_FREQ 100.0f
#define CONTROLLER_FREQ 200.0f
#define METERS_PER_COUNT 0.00047391f
#define PERIODS_PER_SEC CONTROLLER_FREQ
#define METERS_PER_PIXEL 0.002777777778f
#define VIEW_DISTANCE 0.21f
#define MAX_LATERAL_ACCEL 9.81f // got 1.2*g which is silly...setting to g for now...
#define WHEELBASE 0.26f
#define MAX_STEER_RAD 0.65f // TODO remeasure

void clamp(float *var, float min, float max);
void diff(bool *a, bool *d, int len);

#endif
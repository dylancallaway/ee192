#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "mbed.h"
#include "encoder.h"
#include "camera.h"
#include "myutils.h"
#include <vector>

class Estimator
{
    // Access specifier
public:
    // Data members
    Encoder *enc;
    Camera *cam;
    float meas_speed;
    float est_speed;
    float meas_accel;
    float est_accel;
    int *cap;
    float est_deviation, meas_deviation;
    bool off_track = 0;

    // Default constructor
    Estimator() = default;

    // Parameterized constructor
    Estimator(Encoder *enc_in, Camera *camera_in);

    // Member functions
    float get_dist();
    float get_speed();
    float get_accel();
    float get_deviation();
    float get_deviation2();
};

#endif
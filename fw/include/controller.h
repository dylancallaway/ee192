#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"
#include "estimator.h"
#include "motor.h"
#include "servo.h"
#include "myutils.h"

class Controller
{
    // Access specifier
public:
    // Data members
    EventQueue queue;
    Thread thread;
    Estimator *est;
    Motor *mot;
    Servo *servo;
    float pwm_out = 0.0f;
    float est_speed = 0.0f;
    float est_accel = 0.0f;
    float steer_to_set, speed_to_set;
    float est_deviation = 0.0f;
    Timer timer;
    int elapsed_time;

    // Default constructor
    Controller() = default;

    // Parameterized constructor
    Controller(Estimator *est_in, Motor *mot_in, Servo *servo_in);

    // Member functions
    void Start();
    void Stop(); // should implement this
    void ControlLoop();
    void set_speed(float speed);
    float get_speed();
    float get_accel();
    void set_steer(float rads_to_steer);
};

#endif
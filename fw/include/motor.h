#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

#ifndef MOT_PWM_PERIOD_us
#define MOT_PWM_PERIOD_us 50
#endif

class Motor
{
    // Access specifier
public:
    // Data members
    PwmOut mot_a, mot_b;
    DigitalOut mot_en;

    // Default constructor
    Motor() = default;

    // Parameterized constructor
    Motor(PinName pin_a, PinName pin_b, PinName pin_en);

    // Member functions
    void Enable();
    void Disable();
    void Run(float dc);
    void Brake(float dc);
};

#endif
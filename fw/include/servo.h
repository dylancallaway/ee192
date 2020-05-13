#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

#define SERVO_STRAIGHT 1.6
#define SERVO_LEFT 1.75
#define SERVO_RIGHT 1.45

class Servo
{
    // Access specifier
public:
    // Data members
    DigitalOut sig;
    EventQueue queue;
    Thread thread;

    // Default constructor
    Servo() = default;

    // Parameterized constructor
    Servo(PinName sig_in);

    // Member functions
    void GeneratePulse(int us);
    void pulse_in_thread(int us);
};

#endif
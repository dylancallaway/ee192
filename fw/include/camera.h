#ifndef CAMERA_H
#define CAMERA_H

#include "mbed.h"

class Camera
{
    // Access specifier
public:
    // Data members
    DigitalOut clk, si;
    AnalogIn sig;
    int cap[128];

    // Default constructor
    Camera() = default;

    // Parameterized constructor
    Camera(PinName clk_in, PinName si_in, PinName sig_in);

    // Member functions
    int *Capture();
};

#endif
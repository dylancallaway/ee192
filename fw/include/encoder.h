#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"

class Encoder
{
    // Access specifier
public:
    // Data members
    volatile int count = 0;
    int8_t lookup_table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    InterruptIn enc_a, enc_b;

    // Default constructor
    Encoder() = default;

    // Parameterized constructor
    Encoder(PinName pin_a, PinName pin_b);

    // Member functions
    void isr();
    int get_count();
    void reset_count();
};

#endif
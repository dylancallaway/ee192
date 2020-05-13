#include "mbed.h"
#include "encoder.h"

Encoder::Encoder(PinName pin_a, PinName pin_b) : enc_a(pin_a), enc_b(pin_b)
{
    // attach interrupts to all motors+pins on rise and fall
    enc_a.rise(callback(this, &Encoder::isr));
    enc_a.fall(callback(this, &Encoder::isr));
    enc_b.rise(callback(this, &Encoder::isr));
    enc_b.fall(callback(this, &Encoder::isr));
}

void Encoder::isr()
{
    // count += 1;
    static uint8_t enc_val = 0;
    enc_val = (enc_val << 2) | ((enc_a.read() << 1) | enc_b.read());
    count += lookup_table[enc_val & 0b1111];
}

int Encoder::get_count()
{
    return count;
}

void Encoder::reset_count()
{
    count = 0;
}
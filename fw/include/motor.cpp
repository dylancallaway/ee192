#include "mbed.h"
#include "motor.h"

Motor::Motor(PinName pin_a, PinName pin_b, PinName pin_en) : mot_a(pin_a), mot_b(pin_b), mot_en(pin_en)
{
    mot_en.write(1); // 1 is disable

    mot_a.period_us(MOT_PWM_PERIOD_us);
    mot_a.write(0.0f);

    mot_b.period_us(MOT_PWM_PERIOD_us);
    mot_b.write(0.0f);
}

void Motor::Enable()
{
    mot_en.write(0);
}

void Motor::Disable()
{
    mot_en.write(1);
}

void Motor::Run(float dc)
{
    mot_b.write(0.0f);
    mot_a.write(dc);
}

void Motor::Brake(float dc)
{
    mot_a.write(0.0f);
    mot_b.write(dc);
}
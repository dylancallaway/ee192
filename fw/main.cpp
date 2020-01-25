#include "mbed.h"

// Serial
Serial pc(USBTX, USBRX, 115200);

PwmOut grn_led = LED2;

int main()
{
    grn_led.period_ms(1000);
    grn_led.write(0.75f);
}

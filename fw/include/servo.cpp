#include "mbed.h"
#include "servo.h"

Servo::Servo(PinName sig_in)
    : sig(sig_in), queue(32 * EVENTS_EVENT_SIZE), thread(osPriorityRealtime6, 2056)
{
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
}

void Servo::GeneratePulse(int us)
{
    queue.call(callback(this, &Servo::pulse_in_thread), us);
}

void Servo::pulse_in_thread(int us)
{
    sig.write(1);
    wait_us(us);
    sig.write(0);
}

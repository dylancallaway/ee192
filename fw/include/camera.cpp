#include "mbed.h"
#include "camera.h"

Camera::Camera(PinName clk_in, PinName si_in, PinName sig_in)
    : clk(clk_in), si(si_in), sig(sig_in)
{
}

int *Camera::Capture()
{
    clk.write(0);
    si.write(1);
    wait_ns(20);
    for (int i = 0; i < 128; i++)
    {
        clk.write(1); // start pixel cap
        wait_ns(120);
        cap[i] = sig.read_u16();
        si.write(0);
        clk.write(0);
        wait_ns(50);
    }
    clk.write(1);
    wait_ns(50);
    clk.write(0);
    return cap;
}

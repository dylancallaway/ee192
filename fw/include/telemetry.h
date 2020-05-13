#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "mbed.h"
#include "estimator.h"
#include "controller.h"
#include "myutils.h"

class Telemetry
{
public:
    Estimator *est;
    Controller *cont;
    Serial *bt;

    int *cap;
    bool controller_on_flag = 0;

    bool data_transmit_back = 0;

    EventQueue queue;
    Thread thread;

    Timer timer;

    Telemetry() = default;

    Telemetry(Estimator *est_in, Controller *cont_in, Serial *bt_in);

    // Functions
    void CriticalCommandLinkInit();
    void Start();
    void Stop();
    void TelemetryLoop();
    void transmit_csv();
};

#endif
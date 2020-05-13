#include "mbed.h"
#include "telemetry.h"

Telemetry::Telemetry(Estimator *est_in, Controller *cont_in, Serial *bt_in)
    : queue(32 * EVENTS_EVENT_SIZE), thread(osPriorityHigh4, 4096)
{
    est = est_in;
    cont = cont_in;
    bt = bt_in;
}

void Telemetry::CriticalCommandLinkInit()
{
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call_every((int)(1000 / TELEMETRY_FREQ), callback(this, &Telemetry::TelemetryLoop));
    timer.start();
}

void Telemetry::Start()
{
    data_transmit_back = 1;
}

void Telemetry::Stop()
{
    data_transmit_back = 0;
}

void Telemetry::TelemetryLoop()
{
    if (bt->readable())
    {
        int cmd_in = bt->getc();
        switch (cmd_in)
        {
        case 32: // space
        {
            cont->mot->Disable();
            break;
        }
        case 12: // x
        {
            cont->mot->Disable();
            break;
        }
        case 109: // m
        {
            cont->mot->Enable();
            break;
        }
        case 112: // p
        {
            cap = est->cam->Capture();
            bt->printf("\nNew Image!\n");
            for (int i = 0; i < 128; i++)
            {
                bt->printf("%0.2f ", (float)cap[i] / 65536.0f);
            }
            bt->printf("\n");
            break;
        }
        case 101: // e
        {
            bt->printf("\nEst dev: %.3f\n", cont->est_deviation);
            break;
        }
        case 99: // c
        {
            if (controller_on_flag == 0)
            {
                cont->Start();
                controller_on_flag = 1;
            }
            break;
        }
        case 115: // s
        {
            bt->printf("\nSpeed: %2.2f\n", cont->get_speed());
            break;
        }
        case 100: // d
        {
            bt->printf("\nCounts: %d\n", est->enc->get_count());
            bt->printf("\nDistance: %.2f\n", est->get_dist());

            break;
        }
        case 97: // a
        {
            bt->printf("\nAccel: %2.2f\n", cont->get_accel());
            break;
        }
        case 116: // t
        {
            // bt->printf("TEL,est_deviation,steer_to_set,est_speed,speed_to_set,time_ms");
            data_transmit_back = !data_transmit_back;
            // bt->printf("\nTest: %.2f\n", );
            // bt->printf("\nSpeed to Set: %2.2f\n", cont->speed_to_set);
            // bt->printf("\nTime: %d\n", cont->elapsed_time);
            // bt->printf("\nPWM Out: %2.2f\n", cont.pwm_out);
            break;
        }
        default:
            cont->mot->Disable();
            break;
        }
    }
    if (data_transmit_back == 1)
    {
        transmit_csv();
    }
}

void Telemetry::transmit_csv()
{
    bt->printf("\n%.4f,%.4f,%.4f,%.4f,%d", cont->est_deviation, cont->steer_to_set, cont->est_speed, cont->speed_to_set, timer.read_ms());
}
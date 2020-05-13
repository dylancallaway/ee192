#include "mbed.h"
#include "controller.h"

Controller::Controller(Estimator *est_in, Motor *mot_in, Servo *servo_in)
    : queue(32 * EVENTS_EVENT_SIZE), thread(osPriorityRealtime5, 4096)
{
    est = est_in;
    mot = mot_in;
    servo = servo_in;
}

void Controller::Start()
{
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call_every(1000 / CONTROLLER_FREQ, callback(this, &Controller::ControlLoop));
    timer.start();
}

void Controller::ControlLoop()
{
    timer.reset(); // actual loops take about 4998us. 2us prolly down to actually measuring time

    Controller::set_speed(speed_to_set);
    Controller::set_steer(steer_to_set); // 1578 is approx centered // 1578+x is left // +/-190 is max each way

    static float last_error = 0.0f;
    est_deviation = est->get_deviation();
    float error = -est_deviation; // same as error = set_deviation - est_deviation?
    float d_error = (error - last_error) * PERIODS_PER_SEC;
    static float i_error = 0.0f;
    i_error += error / PERIODS_PER_SEC;

    // Steer control
    // float steer_Kp = 0.8f + est_speed / 18.0f;
    static float steer_Kp = 1.0f;
    static float steer_Ki = 1.0f;
    static float steer_Kd = 0.18f;
    float theta = atan(error / VIEW_DISTANCE);
    float steer_p = steer_Kp * theta;
    float steer_i = steer_Ki * i_error;
    clamp(&steer_i, -0.1f, 0.1f);
    float steer_d = steer_Kd * d_error;
    steer_to_set = steer_p + steer_i + steer_d;
    clamp(&steer_to_set, -MAX_STEER_RAD, MAX_STEER_RAD);
    // end steer control

    // Speed control
    float radius = WHEELBASE / sin(abs(steer_to_set)); // using steer to set with D control helps a lot
    static float speed_Kd = 0.0f;
    static float speed_Kp = 1.4f; // 1.4 through the turns is good
    // float speed_Kp = 0.2f + 0.8f / radius;
    float speed_p = speed_Kp * sqrt(MAX_LATERAL_ACCEL * radius);
    float speed_d = speed_Kd * d_error;
    speed_to_set = speed_p + speed_d;
    clamp(&speed_to_set, 0.0f, 3.5f); // 6.5m/s is max measured speed so far
    // end speed control

    last_error = error;

    elapsed_time = timer.read_us(); // taking 3024us right now (image cap is about 1300us so calcs do not take long)
}

void Controller::set_speed(float speed)
{
    static float Kp = 0.0f; // set dynamically later
    static float Ki = 0.01f;
    static float Kd = 0.001f; // numerical derivative is noisy FYI.
    static float p = 0.0f, i = 0.0f, d = 0.0f;

    if (pwm_out >= 0.0f)
    {
        mot->Run(pwm_out);
    }
    else
    {
        mot->Brake(0.3f);
    }

    est_speed = est->get_speed();
    float error = speed - est_speed;

    if (est_speed < 0.5f)
    {
        Kp = 0.1f;
    }
    else
    {
        Kp = 0.5f / est_speed + 0.2f;
    }

    p = Kp * error;

    i += Ki * error;

    if (est_speed <= 0.5f)
    {
        i = 0.0f;
    }

    clamp(&i, -0.3f, 1.0f);

    est_accel = est->get_accel();
    d = -Kd * est_accel;

    pwm_out = p + i + d;

    clamp(&pwm_out, -0.3f, 1.0f);
}

float Controller::get_speed()
{
    return est_speed;
}

float Controller::get_accel()
{
    return est_accel;
}

void Controller::set_steer(float rads_to_steer)
{
    static int center_us = 1578;
    static float us_per_rad = 400.0f; // inner: 348.5978, outer: 471.6324
    int us_to_steer = (int)round(us_per_rad * rads_to_steer);
    servo->GeneratePulse(center_us + us_to_steer);
}
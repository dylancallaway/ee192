#include "mbed.h"
#include "estimator.h"

Estimator::Estimator(Encoder *enc_in, Camera *cam_in)
{
    enc = enc_in;
    cam = cam_in;
}

float Estimator::get_dist()
{
    volatile float meas_dist = (float)(enc->get_count());
    meas_dist *= METERS_PER_COUNT;
    return meas_dist;
}

float Estimator::get_speed()
{
    static float last_est_speed = 0.0f;

    meas_speed = Estimator::get_dist();
    enc->reset_count();

    meas_speed *= PERIODS_PER_SEC;

    // est_speed = meas_speed;
    est_speed = 0.6f * meas_speed + 0.4f * last_est_speed;
    last_est_speed = est_speed;

    return est_speed;
}

float Estimator::get_accel()
{
    static float last_est_speed = 0.0f;
    static float last_est_accel = 0.0f;

    meas_accel = est_speed - last_est_speed;
    last_est_speed = est_speed;

    meas_accel *= PERIODS_PER_SEC;

    // est_accel = meas_accel;
    est_accel = 0.6f * meas_accel + 0.4f * last_est_accel;
    last_est_accel = est_accel;

    return est_accel;
}

float Estimator::get_deviation()
{
    // static float last_est_deviation = 0.0f;

    cap = cam->Capture();

    // Center of line detection
    // This find the "x" centroid of the area greater than some threshold value
    int area = 0;
    int sum = 0;
    int abs_min_threshold = 10000; // TODO add dynamic threshold like max(cap)+min(cap)/2 or something
    int threshold = (int)round((*max_element(cap, cap + 127)) * 0.8f);
    float center_offset = 73.0f;

    for (int i = 0; i < 128; i++)
    {
        if (cap[i] > threshold && cap[i] > abs_min_threshold)
        {
            area += cap[i];
            sum += (i + 1) * cap[i];
        }
    }

    if (area == 0)
    {
        // TODO add a counter to this so if you dont see track for X loops you stop
        meas_deviation = est_deviation;
    }
    else
    {
        float cent = (float)sum / (float)area;
        meas_deviation = cent - center_offset; // can tune the - XX.0f to account for mechanical offset of camera
        meas_deviation *= METERS_PER_PIXEL;
    }

    est_deviation = meas_deviation;

    // est_deviation = 0.6f * meas_deviation + 0.4f * last_est_deviation;
    // last_est_deviation = est_deviation;

    // Positive deviation is to the right, negative to the left

    return est_deviation;
}

float Estimator::get_deviation2() // not working
{
    cap = cam->Capture();

    int abs_min_threshold = 10000;
    int max_val = round(*max_element(cap, cap + 127));
    int min_val = round(*min_element(cap, cap + 127));
    int upper_threshold = (int)(0.8f * max_val);
    int lower_threshold = (int)(1.2f * min_val);
    float center_offset = 73.0f;
    int line_start[10], line_end[10];
    float line_center[10], center_diff[10] = {0.0f};
    bool in_line = 0;
    int line_num = 0;
    static float center, last_center = 0.0f;

    for (int i = 0; i < 128; i++)
    {
        if (i == 127)
        {
            cap[i] = 0;
        }
        if (cap[i] >= abs_min_threshold)
        {
            if (cap[i] >= upper_threshold)
            {
                if (in_line == 0)
                {
                    in_line = 1;
                    line_start[line_num] = i;
                }
                else if (in_line == 1)
                {
                    in_line = 1;
                }
            }
            else if (cap[i] < upper_threshold)
            {
                if (in_line == 0)
                {
                    in_line = 0;
                }
                else if (in_line == 1)
                {
                    in_line = 0;
                    line_end[line_num] = i;
                    line_center[line_num] = (float)(line_start[line_num] + line_end[line_num]) / 2;
                    center_diff[line_num] = line_center[line_num] - last_center;
                    line_num += 1;
                }
            }
        }
    }

    center = line_center[distance(center_diff, min_element(center_diff, center_diff + 9))];

    meas_deviation = center - center_offset;
    meas_deviation *= METERS_PER_PIXEL;
    est_deviation = meas_deviation;

    // Positive deviation is to the right, negative to the left

    last_center = center;

    return est_deviation;
}
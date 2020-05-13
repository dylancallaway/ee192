void clamp(float *var, float min, float max)
{
    if (*var > max)
    {
        *var = max;
    }
    else if (*var < min)
    {
        *var = min;
    }
}

void diff(bool *a, bool *d, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i == 0)
        {
            d[i] = a[i];
        }
        else
        {
            d[i] = (a[i] - a[i - 1]) != 0;
        }
    }
}
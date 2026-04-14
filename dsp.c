#include <stdint.h>
#include <math.h>

float noise_filter(float prev, float next, float noise_limit)
{
    static float filtered = 0;
    if(next - noise_limit > prev)
    {
        filtered = (3*prev + next)/4;
    } else 
    {
        filtered = next;
    }
    return filtered;
}

float moving_avg(float new_value, float alpha)
{
    static float avg = 0.0f;

    // Reject invalid inputs
    if (isnan(new_value) || isnan(avg)) {
        //avg = 1.4;
        avg = (alpha * 0) + (1.0f - alpha) * avg;
        return avg;
    }

    // Clamp alpha
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    avg = alpha * new_value + (1.0f - alpha) * avg;

    return avg;
}

#include "hardware/adc.h"
#include <stdint.h>
#include "functions.h"
#include <math.h>

//ADC read function. Can only be called after adc has been initialized. 
uint16_t readadc(uint16_t channel){
    adc_select_input(channel);
    return adc_read();
}

float press_out(uint16_t adc_step){
    //Pa/step = 2.44140625
    //Avg zero offset = 403.5
    float Pa = adc_step*2.44140625 - 403.5;
    return Pa;
}

float o2_out(uint16_t adc_step){
    //@ 20.9% o2, 3096.5 step avg
    //(measured 23.5k res) percent/steps = 0.00674955595
    float o2 = adc_step * 0.00674955595;
    return o2;
}

float airflow(float Pa, float A1, float A2, float rho){
    //A1 = 26mm, A2 = 20mm,
    //rho = 1.225
    //rho = 1.292 density at 0°C, MSL, 1013.25 hPa, dry air
    // rho = 1.123 BTPS conditions: density at ambient  pressure, 35°C, 95% humidity
    float area_ratio = A2 / A1;
    float denom = rho * (1.0f - area_ratio * area_ratio);

    float velocity = sqrtf((2.0f * Pa) / denom);

    float Q = A2 * velocity;

    return Q;

}

float moving_avg(float new_value, float alpha)
{
    static float avg = 0.0f;

    avg = alpha * new_value + (1.0f - alpha) * avg;

    return avg;
}
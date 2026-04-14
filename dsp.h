#ifndef DSP
#define DSP 

#include <stdint.h>


float noise_filter(float prev, float new, float noise_limit);
float moving_avg(float next, float alpha);


#endif
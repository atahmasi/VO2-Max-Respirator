#ifndef FUNCTIONS
#define FUNCTIONS 

#include <stdint.h>

uint16_t readadc(uint16_t channel);
float press_out(uint16_t adc_step);
float o2_out(uint16_t adc_step);
float airflow(float Pa, float A1, float A2, float rho);



#endif
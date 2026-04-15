/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "functions.h"
#include "ble.h"
#include "dsp.h"
#include "spi_mcp3204.h"
#include "isr.h"

//#include "btstack_config_common.h"
//#include "btstack_config.h"
#define PI 3.14159265359f

// Venturi parameters
const float D1 = 26e-3f;
const float D2 = 20e-3f;

const float A1 = PI * (D1 * D1) / 4.0f;
const float A2 = PI * (D2 * D2) / 4.0f;

const float rho = 1.225f;    // air density
const float alpha = 0.1f;   // smoothing factor

extern volatile uint16_t current_ble_val;


// Core 1: sampling + processing
void core1_entry() {
    static int print_divider = 0;

    uint16_t rpress = 0;
    uint16_t ro2    = 0;

    float Pa = 0;
    float o2_diff = 0;

    float Q = 0;
    float Q_denoise = 0;

    float Q_avg = 0;
    float o2_avg = 0;
    float p_avg = 0;

    // ADC & isr initialization
    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1
    mcp3204_init();
    isr_init();

    while (true) {

        //Read ADC channels
        //rpress = readadc(1);
       // ro2    = readadc(0);
       /* rpress = mcp3204_read(1);
        ro2 = mcp3204_read(0);*/

        // Isr; skip unless sample is ready
        if (!sample_ready) continue;
        sample_ready = false;

        // Convert ADC values
        Pa = press_out(rpress_sample);
        o2_diff = o2_out(ro2_sample);

        // Calculate airflow 
        Q = airflow(Pa, A1, A2, rho);

        //Detect and remove large jumps from noise
        Q_denoise = noise_filter(Q_denoise, Q, 100);

        // Smooth variables 
        Q_avg = moving_avg(Q_denoise, alpha);
        o2_avg = moving_avg(o2_avg, alpha);
        p_avg = moving_avg(p_avg, alpha);

        current_ble_val = (uint16_t)(Q_avg * o2_diff/77);

        // Print on everey x many loops
        print_divider++;

        if (print_divider >= 100)
        {   
            print_divider = 0;

            printf("Pressure: %.2f Pa  O2: %.2f %%  Flow: %.5f  AvgFlow: %.5f VO2: %u\n",
                p_avg, o2_avg, Q, Q_avg, current_ble_val);
        }
        
        //current_ble_val = 5;
        //setting to 5 works.
    }
}

//Core 0, BLE
int main() {

    stdio_init_all();
    sleep_ms(2000);

    ble_init();

    multicore_launch_core1(core1_entry);

    while (true) {
        ble_process();
    }
}

//look into filtering o2 and pressure prior to putting into flow calc.  
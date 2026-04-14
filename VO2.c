/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "functions.h"
#include "ble.h"
#include "dsp.h"
#include "spi_mcp3204.h"

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
int main() {

    uint16_t rpress = 0;
    uint16_t ro2    = 0;

   
    float Pa = 0;
    float o2_diff = 0;

    float Q = 0;
    float Q_denoise = 0;


    float Q_avg = 0;

    stdio_init_all();
    sleep_ms(2000);

    // ADC initialization
    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1
    mcp3204_init();
    ble_init();

    while (true) {

        // --- Read ADC channels ---
        rpress = readadc(1);
        ro2    = readadc(0);
       /* rpress = mcp3204_read(1);
        ro2 = mcp3204_read(0);*/

        // --- Convert ADC values ---
        Pa = press_out(rpress);
        o2_diff = o2_out(ro2);

        // --- Calculate airflow ---
        Q = airflow(Pa, A1, A2, rho);

        // --- Detect and remove large jumps from noise ---
        Q_denoise = noise_filter(Q_denoise, Q, 100);

        // --- Smooth airflow ---
        Q_avg = moving_avg(Q_denoise, alpha);

        current_ble_val = (uint16_t)(Q_avg * o2_diff/77);

        // --- Output ---
        printf("Pressure: %.2f Pa  O2: %.2f %%  Flow: %.5f  AvgFlow: %.5f VO2: %u\n",
               Pa, o2_diff, Q, Q_avg, current_ble_val);
        
        //current_ble_val = 5;
        //setting to 5 works.
        ble_process();
        sleep_ms(100);
    }
}

//note to self: look into the static const use of profile_data, it creates multiple copies per file. 
//look into filtering o2 and pressure prior to putting into flow calc.  
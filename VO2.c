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

//#include "btstack_config_common.h"
//#include "btstack_config.h"
#define PI 3.14159265359f

// Venturi parameters
const float D1 = 26e-3f;
const float D2 = 20e-3f;

const float A1 = PI * (D1 * D1) / 4.0f;
const float A2 = PI * (D2 * D2) / 4.0f;

const float rho = 1.225f;    // air density
const float alpha = 0.15f;   // smoothing factor

extern uint16_t current_ble_val;
int main() {
    stdio_init_all();
    sleep_ms(2000);

    // ADC initialization
    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1

    ble_init();

    while (true) {

        // --- Read ADC channels ---
        uint16_t rpress = readadc(1);
        uint16_t ro2    = readadc(0);

        // --- Convert ADC values ---
        float Pa = press_out(rpress);
        float o2 = o2_out(ro2);

        // --- Calculate airflow ---
        float Q = airflow(Pa, A1, A2, rho);

        // --- Smooth airflow ---
        float Q_avg = moving_avg(Q, alpha);

        // --- Output ---
        printf("Pressure: %.2f Pa  O2: %.2f %%  Flow: %.5f  AvgFlow: %.5f\n",
               Pa, o2, Q, Q_avg);
        
        //current_ble_val = (uint16_t)Q_avg;
        current_ble_val = 5;
        ble_process();
        sleep_ms(100);
    }
}

//note to self: look into the static const use of profile_data, it creates multiple copies per file. 
//also, look into changing value sent from decimal to uintwhatever similar to example; currently stuck on 0.
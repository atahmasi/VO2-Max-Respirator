/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "functions.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);

    // ADC initialization
    adc_init();
    adc_gpio_init(26); // ADC0
    adc_gpio_init(27); // ADC1

    // Venturi parameters
    const float A1 = 26e-3f;     // 26 mm
    const float A2 = 20e-3f;     // 20 mm
    const float rho = 1.225f;    // air density

    const float alpha = 0.05f;   // smoothing factor

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

        sleep_ms(100);
    }
}
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "functions.h"

#define AVG_SAMPLES0 300
#define AVG_SAMPLES1 300

int main() {
    stdio_init_all();
    sleep_ms(2000);  

    //adc init
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);




    const float VREF = 3.3f;
    const float ADC_MAX = 4095.0f; // 12-bit

    uint16_t buffer0[AVG_SAMPLES0] = {0};
    uint32_t sum0 = 0;
    int index0 = 0;


    uint16_t buffer1[AVG_SAMPLES1] = {0};
    uint32_t sum1 = 0;
    int index1 = 0;



    while (true) {
        adc_select_input(0);
        uint16_t rpress = adc_read();

        // Remove oldest sample
        sum1 -= buffer1[index1];

        // Store new sample
        buffer1[index1] = rpress;

        // Add new sample
        sum1 += rpress;

        // Move circular index
        index1 = (index1 + 1) % AVG_SAMPLES1;

        // Compute averaged value
        float avg_raw1 = (float)sum1 / AVG_SAMPLES1;
        float voltage1 = (avg_raw1 * VREF) / ADC_MAX;

        printf("Raw: %4u  AvgRaw: %.2f  Voltage: %.3f V\n", rpress, avg_raw1, voltage1);

        sleep_ms(10);
    }
}

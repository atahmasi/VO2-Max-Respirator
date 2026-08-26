#ifndef ISR_H
#define ISR_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/timer.h"

typedef struct {
    uint16_t rpress;
    uint16_t ro2;
} adc_sample_t;

extern volatile adc_sample_t adc_buffers[2];
extern volatile uint8_t ready_buffer;
extern volatile bool sample_ready;

void isr_init(void);

#endif
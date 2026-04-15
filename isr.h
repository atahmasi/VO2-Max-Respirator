#ifndef ISR_H
#define ISR_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/timer.h"

extern volatile uint16_t rpress_sample;
extern volatile uint16_t ro2_sample;
extern volatile bool sample_ready;

// Init and start the sampling timer
void isr_init(void);

#endif
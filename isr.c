#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "isr.h"
#include "functions.h"
#include "pico/time.h"

volatile uint16_t rpress_sample = 0;
volatile uint16_t ro2_sample = 0;
volatile bool sample_ready = false;

// Timer instance
static struct repeating_timer sample_timer;

// ISR callback
static bool sample_timer_callback(struct repeating_timer *t) {

    rpress_sample = readadc(1);
    ro2_sample    = readadc(0);

    sample_ready = true;

    return true; // keep repeating
}

// Initialize and start ISR timer
void isr_init(void) {
    // 100 Hz sampling 
    add_repeating_timer_ms(-10, sample_timer_callback, NULL, &sample_timer);
}
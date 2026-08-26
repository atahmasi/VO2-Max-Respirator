#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "isr.h"
#include "functions.h"

volatile adc_sample_t adc_buffers[2];
volatile uint8_t ready_buffer = 0;
volatile bool sample_ready = false;

static struct repeating_timer sample_timer;
static uint8_t write_buffer = 0;

static bool sample_timer_callback(struct repeating_timer *t)
{
    adc_buffers[write_buffer].rpress = readadc(1);
    adc_buffers[write_buffer].ro2 = readadc(0);

    ready_buffer = write_buffer;
    write_buffer ^= 1;
    sample_ready = true;

    return true;
}

void isr_init(void)
{
    add_repeating_timer_ms(-10, sample_timer_callback, NULL, &sample_timer);
}
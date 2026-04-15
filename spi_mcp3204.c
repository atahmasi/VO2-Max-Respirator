#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "spi_mcp3204.h"



static inline void cs_select() {
    gpio_put(PIN_CS, 0);
}

static inline void cs_deselect() {
    gpio_put(PIN_CS, 1);
}

void mcp3204_init() {
    
    spi_init(SPI_PORT, 1000 * 1000); // 1 MHz
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    cs_deselect(PIN_CS);

    gpio_put(PIN_CS, 1);
    sleep_ms(10);
}


uint16_t mcp3204_read(uint8_t channel) {
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = 0x06;
    tx[1] = (channel & 0x03) << 6;
    tx[2] = 0x00;

    printf("TX: %02X %02X %02X\n", tx[0], tx[1], tx[2]);

    gpio_put(PIN_CS, 0);
    sleep_us(1);  // important
    spi_write_read_blocking(SPI_PORT, tx, rx, 3);
    gpio_put(PIN_CS, 1);

    printf("RX: %02X %02X %02X\n", rx[0], rx[1], rx[2]);

    return ((rx[1] & 0x0F) << 8) | rx[2];
}

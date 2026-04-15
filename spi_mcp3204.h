#ifndef SPI_MCP3204_H
#define SPI_MCP3204_H

#include <stdio.h>

#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void mcp3204_init();
uint16_t mcp3204_read(uint8_t channel);

#endif
#ifndef BLE_H
#define BLE_H

#include <stdint.h>
#include <stdbool.h>
//#include "btstack_config_common.h"
//#include "btstack_config.h"
#include "vo2_gatt.h"
int ble_init(void);
void ble_process(void);



extern volatile uint16_t current_ble_val;
extern const uint8_t profile_data[];
#endif
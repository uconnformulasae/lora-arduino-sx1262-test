#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

#include <Arduino.h>

uint8_t serialInputCollectYN();

String serialInputListener();

void serialPrintOptions(float frequency, float bandwidth, uint8_t spreading_factor, uint8_t coding_rate, uint8_t sync_word, uint8_t output_power, uint16_t preamble_length);

#endif
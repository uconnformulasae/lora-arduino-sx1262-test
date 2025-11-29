/**
 *  NAME: serial_utils.h
 *  Description: Holds basic helper/utility functions to handle common serial monitor tasks
 * 
 *  Author: Charlie K.
 *  Date: 11/26/25
 */

#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

#include <Arduino.h>

uint8_t serialInputCollect(int numOptions, ...);

uint8_t serialInputCollectYN();

String serialInputListener();

void serialPrintOptions(float frequency, float bandwidth, uint8_t spreading_factor, uint8_t coding_rate, uint8_t sync_word, uint8_t output_power, uint16_t preamble_length);

uint8_t serialModeMenu();

#endif
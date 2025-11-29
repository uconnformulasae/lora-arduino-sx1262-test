/**
 *  NAME: serial_utils.h
 *  Description: Holds basic helper/utility functions to handle common serial monitor tasks
 * 
 *  Author: Charlie K.
 *  Date: 11/29/25
 */

#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

#include <Arduino.h>

float serialInputCollectFloat();

uint8_t serialInputCollectOption(int numOptions, ...);

uint8_t serialInputCollectOptionYN();

String serialInputListener();

void serialPrintOptions(float frequency, float bandwidth, uint8_t spreading_factor, uint8_t coding_rate, uint8_t sync_word, uint8_t output_power, uint16_t preamble_length);

uint8_t serialModeMenu();

uint8_t serialRxTxSelector();

#endif
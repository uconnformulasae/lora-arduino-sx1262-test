/**
 *  NAME: utils.h
 *  Description: Holds basic utility functions
 * 
 *  Author: Charlie K.
 *  Date: 2/4/2026
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <RadioLib.h>

#define EPSILON 0.001f

typedef struct settings {
    //uint32_t signature; For future use
    float base_freq;
    float bw;
    uint8_t sf;
    uint8_t cr;
    uint16_t sw;
    uint8_t pwr;
    uint16_t pre;
    uint32_t seed; 
} settings;

// void serialCommandParser(String input, settings radset); For future use

void eepromSaveSettings(settings radset);

void serialError(const char* msg, char* value);

void serialError(const char* msg, int value);

uint8_t isClose(float a, float b);

void userInitializeSX1262(SX1262 radio, settings radset, int rf_sw_pin);

#endif
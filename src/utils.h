/**
 *  NAME: utils.h
 *  Description: Holds basic utility functions
 * 
 *  Author: Charlie K.
 *  Date: 11/29/25
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#define EPSILON 0.001f

uint8_t isClose(float a, float b);

#endif
/**
 *  NAME: utils.cpp
 *  Description: Holds basic utility functions 
 * 
 *  Author: Charlie K.
 *  Date: 11/29/25
 */

#include <Arduino.h>
#include <utils.h>

uint8_t isClose(float a, float b) { // Float comparison cuz its difficult with approximations and whatnot. Honestly, AI helped me a decent bit with this.
    if (fabs(a - b) < EPSILON) {return 1;}
    else {return 0;}
}
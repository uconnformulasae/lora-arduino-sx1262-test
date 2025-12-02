/**
 *  NAME: modes.h
 *  Description: Includes the routines for the different modes
 * 
 *  Author: Charlie K.
 *  Date: 12/2/25
 */

#ifndef MODES_H
#define MODES_H

#include <Arduino.h>
#include <RadioLib.h>

void benchModeTx(SX1262 radio);

void benchModeRx(SX1262 radio);

void pingModeTx(SX1262 radio);

void pingModeRx(SX1262 radio);

void manualMode(SX1262 radio);

void manualModeTx(SX1262 radio, String msg);

void manualModeRx(SX1262 radio);

void manualModeRxInterruptAction();

#endif
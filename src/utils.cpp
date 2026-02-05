/**
 *  NAME: utils.cpp
 *  Description: Holds basic utility functions 
 * 
 *  Author: Charlie K.
 *  Date: 11/29/25
 */

#include <Arduino.h>
#include <utils.h>
#include <EEPROM.h>


void serialError(const char* msg, char* value) {
    Serial.print(msg);
    Serial.println(value);
}

void serialError(const char* msg, int value) {
    Serial.print(msg);
    Serial.println(value);
}

void eepromSaveSettings(settings radset) {
    EEPROM.put(0, radset);
    return;
}

uint8_t isClose(float a, float b) { // Float comparison cuz its difficult with approximations and whatnot. Honestly, AI helped me a decent bit with this.
    if (fabs(a - b) < EPSILON) {return 1;}
    else {return 0;}
}

void userInitializeSX1262(SX1262 radio, settings radset, int rf_sw_pin) {
    int radio_state = radio.begin(radset.base_freq, radset.bw, radset.sf, radset.cr, radset.sw, radset.pwr, radset.pre, 1.7F /* Added TCXO voltage for troubleshooting, may not be nessesary*/);

    if (radio_state != RADIOLIB_ERR_NONE) {
        serialError("SX1262 Initialization Error:", radio_state);
    }

    radio.setRfSwitchPins(rf_sw_pin, -1);
}
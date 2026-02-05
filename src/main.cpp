/**
 *  NAME: main.cpp
 *  Description: Main c++ code file
 * 
 *  Author: Charlie K.
 *  Date: 1/25/25
 */

#include <Arduino.h>
#include <RadioLib.h>
#include <config.h>
#include <utils.h>
#include <fhss.h>

SX1262 radio = new Module(SPI_NSS, DIO1, NRST, BUSY);
settings radsettings = {915.0, 62.5, 9, 4, 0x12, 22, 8, 12345};
String data;


void setup() {
    Serial.begin(9600);

    while (!Serial) { // Wait for serial monitor to start before moving on
      delay(1000);
    }
    Serial.println("Serial Monitor Start");

    userInitializeSX1262(radio, radsettings, RF_SW_PIN);

    initializeFHSS(radio, radsettings.seed);
}

void loop() {
    // == Start Transmit Code ==
    String command_input = "123456 Hello Testing, FHSS";
    txFHSS(radio, command_input, sizeof(command_input));
    // == End Transmit Code ==

    // Comment out either the stuff above or below for recieve or transmit

    // == Start Recieve Code ==
    if (rxFHSS(radio, data) == 1) {Serial.println(data);}
    // == End Recieve Code ==
}
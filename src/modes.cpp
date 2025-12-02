/**
 *  NAME: modes.cpp
 *  Description: Includes the routines for the different modes
 * 
 *  Author: Charlie K.
 *  Date: 12/2/25
 */
#include <modes.h>

#include <Arduino.h>
#include <serial_utils.h>
#include <RadioLib.h>

volatile uint8_t rxFlag = 0;

void manualMode(SX1262 radio) {
  String commandInput = serialInputListener();

  if (commandInput[0] != -1) {
    manualModeTx(radio, commandInput);
    radio.startReceive();
  }

  if (rxFlag == 1) {
    rxFlag = 0;

    manualModeRx(radio);
  }
}

void manualModeTx(SX1262 radio, String msg) {
  int stateTX = radio.transmit(msg);

  if (stateTX == RADIOLIB_ERR_NONE) {
    Serial.println("Tx: " + msg);
  } else {
    Serial.print("Transmit Failed. Code: ");
    Serial.println(stateTX);
  }
}

void manualModeRx(SX1262 radio) {
  String rxInput;

  int stateRX = radio.readData(rxInput);

  if (stateRX == RADIOLIB_ERR_NONE) {
    Serial.println("Rx: " + rxInput);
    Serial.println("- RSSI: " + String(radio.getRSSI(), 2));
    Serial.println("- SNR: " + String(radio.getSNR(), 2));
  } else {
    Serial.print("Recieve Failed. Code: ");
    Serial.println(stateRX);
  }
}

void manualModeRxInterruptAction() {
  rxFlag = 1;
}
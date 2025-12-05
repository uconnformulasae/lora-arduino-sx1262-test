/**
 *  NAME: main.cpp
 *  Description: Main c++ code file
 * 
 *  Author: Charlie K.
 *  Date: 12/1/25
 */

#include <Arduino.h>
#include <RadioLib.h>
#include <config.h>
#include <serial_utils.h>
#include <modes.h>

SX1262 radio = new Module(SPI_NSS, DIO1, NRST, BUSY);

void userInitializeSX1262();

void setup() {
  Serial.begin(9600);

  while (!Serial) { // Wait for serial monitor to start before moving on
    delay(1000);
  }
  Serial.println("Serial Monitor Start");

  uint8_t mode = serialModeMenu();

  uint8_t RxTxMode;

  if (mode == 0 || mode == 1) { // Benchmark and Ping Mode, Ask for Rx/Tx
    RxTxMode = serialRxTxSelector();

  } if (mode == 1 || mode == 2) { // Ping and Manual Mode, initalize using user or default settings
    userInitializeSX1262();
  }

  if (mode == 0 && RxTxMode == 1) { // Benchmark Tx side
    // initialize
    // Run script
  }

  if (mode == 0 && RxTxMode == 0) { // Benchmark Rx side
    // initialize
    // Listen for script and print
  }

  if (mode == 1 && RxTxMode == 1) { // Ping Tx side
    Serial.println("Ping mode interval time (float, seconds)?");
    unsigned int interval = (int)(serialInputCollectPingInterval()*1000.0F);

    while (true) {
      pingModeTx(radio);
      delay(interval);
    }
       
  }

  if (mode == 1 && RxTxMode == 0) { // Ping Rx side
    while (true) {
      pingModeRx(radio);
    }
  } else {
    radio.setPacketReceivedAction(manualModeRxInterruptAction);
    radio.startReceive();
    while (true) {
      manualMode(radio);

      delay(10);
    }
  }
 
}

void loop() {
  // Intentionally Blank
}

void userInitializeSX1262() {
  Serial.println("Initializing SX1262...");
  delay(500);
  Serial.println("========== [Default Options] ==========");
  serialPrintOptions(d_frequency, d_bandwidth, d_spreading_factor, d_coding_rate, d_sync_word, d_output_power, d_preamble_length);
  Serial.println("Use Defaults? (Y/n)");

  uint8_t userSelect = serialInputCollectOptionYN();

  int radio_state;
  
  if (userSelect == 1) {
    Serial.println("Using Defaults");

    radio_state = radio.begin(d_frequency, d_bandwidth, d_spreading_factor, d_coding_rate, d_sync_word, d_output_power, d_preamble_length);
  } else {
    Serial.println("Okay, Not Using Defaults");

    Serial.println("========== [Custom Options] ==========");
    Serial.println("*See documentation for more information on options*");
    Serial.println("Frequency (150 MHz through 960 MHz, IMPORTANT: Ensure to set to regional legal frequency, transmission on illegal/incorrect frequencies can be a crime!!)?");
    float freq = serialInputCollectFreq();

    Serial.println("Bandwidth (7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0, or 500.0 kHz)?");
    float band = serialInputCollectBand();

    Serial.println("Spreading Factor (5 through 12)?");
    int8_t sf = serialInputCollectSF();

    Serial.println("Coding Rate (1 through 4)?");
    int8_t cr = serialInputCollectCR();

    Serial.println("Sync Word (2 digit hex)?");
    Serial.print("0x");
    int16_t sw = serialInputCollectSW();

    Serial.println("Output Power (0dBm through 22dBm)?");
    int8_t op = serialInputCollectOP();

    Serial.println("Preamble Length (10 through 65535)?");
    int32_t pl = serialInputCollectPL();

    Serial.println("========== [Final Custom Options] ==========");
    serialPrintOptions(freq, band, sf, cr, sw, op, pl);
    Serial.println("Final Confirmation (y/n)?");

    if (serialInputCollectOptionYN() != 1) {
      while (true) {
        Serial.println("Reset device to change options!");
        delay(60000);
      }
    }

    radio_state = radio.begin(freq, band, sf, cr, sw, op, pl);
  }

  if (radio_state == RADIOLIB_ERR_NONE) {
    Serial.println("Initialization Complete.");
    return;
  } else {
    Serial.print("Initialization Failed. Code: ");
    Serial.println(radio_state);
    while (true) {
        Serial.println("Reset device!");
        delay(60000);
      }
  }
}
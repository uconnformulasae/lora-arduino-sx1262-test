/**
 *  NAME: main.cpp
 *  Description: Main c++ code file
 * 
 *  Author: Charlie K.
 *  Date: 11/26/25
 */

#include <Arduino.h>
#include <RadioLib.h>
#include <config.h>
#include <serial_utils.h>

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
    // Listen for script and print
  }

  if (mode == 1 && RxTxMode == 1) { // Ping Tx side
    // Ask for ping time
    // Run pings
    // Recive responses
  }

  if (mode == 1 && RxTxMode == 0) { // Ping Rx side
    // Listen for pings and respond
  }

  // Manual mode will just end up at main loop below

}

void loop() { // Manual mode
  String commandInput = serialInputListener();

  if (commandInput[0] != -1) {
    Serial.println("Tx: " + commandInput);
  }
  
}

void userInitializeSX1262() {
  Serial.println("Initializing SX1262...");
  delay(500);
  Serial.println("========== [Default Options] ==========");
  serialPrintOptions(d_frequency, d_bandwidth, d_spreading_factor, d_coding_rate, d_sync_word, d_output_power, d_preamble_length);
  Serial.println("Use Defaults? (Y/n)");

  uint8_t userSelect = serialInputCollectYN();
  
  if (userSelect == 1) {
    Serial.println("Using Defaults");

    int radio_state = radio.begin(d_frequency, d_bandwidth, d_spreading_factor, d_coding_rate, d_sync_word, d_output_power, d_preamble_length);
  } else {
    Serial.println("Not Using Defaults");

    Serial.println("Set Following Settings");
    Serial.println("Frequency (150 MHz through 960 MHz, IMPORTANT: Ensure to set to regional legal frequency, transmission on illegal/incorrect frequencies can be a crime!!)?");
  }

  Serial.println("Initialization Complete.");
}
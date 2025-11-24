#include <Arduino.h>
#include <RadioLib.h>
#include <config.h>
#include <serial_utils.h>

SX1262 radio = new Module(SPI_NSS, DIO1, NRST, BUSY);

void setup() {
  Serial.begin(9600);

  while (!Serial) { // Wait for serial monitor to start before moving on
    delay(1000);
  }
  Serial.println("Serial Monitor Start");

  initializeSX1262()
  
  /**TODO: 
   * Ask user what communication mode should be used (basic UDP-like, constant ping, TCP-like back and forth) and any applicable settings
   */
}

void loop() {
  String commandInput = serialInputListener();

  if (commandInput[0] != -1) {
    Serial.println("Tx: " + commandInput);
  }
  
}

void initializeSX1262() {
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

    // TODO: Add custom settings and initialize
  }

  Serial.println("Initialization Complete.");
}
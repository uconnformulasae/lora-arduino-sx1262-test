#include <Arduino.h>

/* Function Prototypes */

String serialInputListener();

/* Setup Routine */

void setup() {
  Serial.begin(9600);

  while (!Serial) { // Wait for serial monitor to start before moving on
    delay(1000);
  }
  Serial.println("Serial Monitor Start");
}

/* Main Routine */

void loop() {
  String commandInput = serialInputListener();

  if (commandInput[0] != -1) {
    Serial.println("Tx: " + commandInput);
  }
  
}

/* Function Definitions */

String serialInputListener() {
  /**
   * Checks if serial buffer has new data. If new data is present, function handles automatic echo to serial terminal and waits until \n is sent to return string. If no data in buffer, returns -1
   * 
   * No Parameters
   * 
   * Returns type String
   */

  char incoming_char = -1;
  String read_buffer;
  
  if (Serial.available() > 0) {           // Check if new data in buffer
      while (incoming_char != '\n') {
        incoming_char = Serial.read();    // Read oldest byte and remove from buffer

        if (incoming_char != -1) {        // Serial.read() returns -1 if no new byte
          read_buffer += incoming_char;   // Add new character to string
          Serial.write(incoming_char);    // Echo back character to terminal
        }
      }
      read_buffer.trim();                 // Trim starting and ending whitespace including \n
  } else {
    read_buffer = incoming_char;          // Set output string to -1 if no data in buffer
  }

  return read_buffer;
}
/**
 *  NAME: serial_utils.cpp
 *  Description: Holds basic helper/utility functions to handle common serial monitor tasks
 * 
 *  Author: Charlie K.
 *  Date: 11/26/25
 */

#include <Arduino.h>
#include <serial_utils.h>

uint8_t serialInputCollect(int numOptions, ...) {
  /**
   * Retrieves a user response for list of options. Returns a uint8_t of option starting with 0. No default option, must provide input matching option (caller can specify a blank "" option to handle if desired but will output as separate option). This function is blocking.
   * 
   * Parameters: char* 's
   * 
   * Returns: uint8_t of option in arguments (staring at 0)
   */
  va_list options;
  va_start(options, numOptions);

  char** optionList; // List to hold options bc va_list can only iterate once

  for (int i = 0; i < numOptions; i++) {
    optionList[i] = va_arg(options, char*);
  }

  va_end(options); // Close list for memory savings

  String commandInput;
  int8_t userInput = -1;
  
  while (userInput == -1) { // Blocking component of function, runs until valid input
    do {
      commandInput = serialInputListener();
    } while (commandInput[0] == -1); // Wait for input from non-blocking function

    commandInput.toLowerCase(); // Normalize text for easier comparison

    for (int i = 0; i < numOptions; i++) {
      if (commandInput == optionList[i]) {  // Check for input in options and return option index number of matching option
        userInput = i;
        break;  // No need to check the rest of the options
      }
    }

    if (userInput == -1) {  // Non-matching input error message
      Serial.println("Invalid Input. Try again.");
    }
  }
  
  return userInput;
}

uint8_t serialInputCollectYN() {
  /**
   * Retrieves a user response for yes or no. Returns a uint8_t of 1 or 0 for yes or no. This function is blocking.
   * 
   * No parameters
   * 
   * Returns 0 or 1 of type uint8_t
   */

  String commandInput;
  int8_t userInput = -1;
  
  while (userInput == -1) { // Blocking component of function, runs until valid input
    do {
      commandInput = serialInputListener();
    } while (commandInput[0] == -1); // Wait for input from non-blocking function

    commandInput.toLowerCase(); // Normalize text for easier comparison

    // Check for yes or no of various types, and return proper value
    if (commandInput == "yes" || commandInput == "y" || commandInput == "" /* TODO: add parameter to control the default option */) {
      userInput = 1;
    } else if (commandInput == "no" || commandInput == "n") {
      userInput = 0;
    } else {
      Serial.println("Invalid Input. Try again.");
    }
  }

  return userInput;
}

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

void serialPrintOptions(float frequency, 
                        float bandwidth, 
                        uint8_t spreading_factor, 
                        uint8_t coding_rate, 
                        uint8_t sync_word, 
                        uint8_t output_power, 
                        uint16_t preamble_length) {
  /**
   * Writes to serial all options for the radio. No returns.
   */
  Serial.println("Frequency: " + String(frequency, 1) + " MHz");
  Serial.println("Bandwidth: " + String(bandwidth, 1) + " kHz");
  Serial.println("Spreading Factor: " + String(spreading_factor));
  Serial.println("Coding Rate: " + String(coding_rate));
  Serial.println("Sync Word: 0x" + String(sync_word, HEX));
  Serial.println("Output Power: " + String(output_power) + " dBm");
  Serial.println("Preamble Length: " + String(preamble_length) + " symbols");
}

uint8_t serialModeMenu() {
  /**
   * Writes to serial asking for user to select between benchmark, ping, manual mode
   */
  Serial.println("Which operating mode should be used?");
  Serial.println("- Benchmark Mode (B) = Used to run through preloaded set of options to determine best options for current conditions,");
  Serial.println("- Ping (P) = Used to test for continuos connection with specified settings by sending periodic ping messages to determine reliability,");
  Serial.println("- Manual (M) = Used to send messages manually using specified settings.");
  Serial.println("*Note* Benchmark and Ping mode both have specific Tx and Rx sides, Manual acts in a tranciever type fasion where each side recieves unless user sends transmit command.");
  Serial.print("Select Option (b/p/M): ");
  return serialInputCollect(3, "b", "p", "m"); 
}
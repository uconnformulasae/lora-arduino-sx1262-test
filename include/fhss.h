/*
 *  NAME: fhss.h
 *  Description: Includes the functions for fhss
 * 
 *  Author: Charlie K.
 *  Date: 1/25/26
 */

#ifndef FHSS_H
#define FHSS_H

#include <Arduino.h>
#include <RadioLib.h>

/**
 * @brief In ms, used to determine the max amount of time allowed to sit on a channel before switching.
 */
#define max_hop_time 400

/**
 * @brief In ms, used to determine how long after recieveing a packet should the sync be considered "lost".
 */
#define max_sync_timeout 30000

/**
 * @brief Array which stores the preset frequencies, consistent across runs so is storred in program memory due to large size.
 */
extern const float fhss_freq_table[50] PROGMEM;

/**
 * @brief Map to store the randomized order of the frequency hopping, must be used to not sequentially go through all frequencies (for fcc compliance).
 */
extern uint8_t fhss_index_map[50];

/**
 * @brief Global variable to store the base time (including any correction) for use to calculate current frequecy hop.
 */
extern unsigned long base_time;

/**
 * @brief Flag bool which stores if an interrupt occurs. 
 */
extern volatile bool rxFHSSFlag;

/**
 * @brief Bool to store if a packet which can be used to sync has been recieved since the timeout of max_sync_timeout
 */
extern bool synced;

/**
 * @brief Used to store what the current hop index is. Used to check if the freq should hop.
 */
extern uint8_t fhss_current_index;

/**
 * @brief Used to store what the previous hop index is. Used to check if the freq should hop. Additionally used for synchronization.
 */
extern uint8_t fhss_previous_index;

/**
 * @brief Used to store the time when the last packet was recieved, used for when a packet hasn't been recieved in awhile and to check against the timeout max_sync_timeout. Resets sync status if too long ago.
 */
extern unsigned long last_packet_time;

/**
 * @brief Global variable to hold the max packet size (not including things like preamble) to prevent going over the max transmission time
 */
extern uint8_t max_packet_size;

/**
 * @brief Generates and shuffles the basic FHSS index map. Randomizes based on the seed in the first 4 bytes of the EEPROM (long type).
 * 
 * @param radio SX1262 radio object connected to the SPI radio
 * 
 * @return void
 */
void initializeFHSS(SX1262& radio);

/**
 * @brief Generates and shuffles the basic FHSS index map. Randomizes based on the seed provided.
 * 
 * @param radio SX1262 radio object connected to the SPI radio
 * @param seed The seed to be used to randomize the shuffle sequence
 * 
 * @return void
 */
void initializeFHSS(SX1262& radio, long seed);

/**
 * @brief Generates an index map from 0 to number of elements needed to perform shuffling
 * 
 * @param map The index array to be filled
 * @param elements The number of elements in the index array
 * 
 * @return void
 */
void generateIndexMap(uint8_t* map, uint8_t elements);

/**
 * @brief Takes a filled index map and shuffles the elements around based on the seed. Note that this only exchanges places of each element and does not duplicate/remove elements.
 * 
 * @param map The index array to be shuffled
 * @param elements The number of elements in the index array
 * @param seed The randomization seed to be used. The seed can be used to generate the same order of elements 
 */
void randomizeIndexMap(uint8_t* map, uint8_t elements, long seed);

/**
 * @brief Simple helper function to grab the frequency which corresponds to the index held at the index in the map. Designed to take sequential numbers
 * 
 * @param fhss_current_index The index of the desired index for the frequency array
 */
float getFreqFHSS(uint8_t fhss_current_index);

/**
 * @brief Interrupt handler function for when the radio indicates data has been recieved
 * 
 * @param void
 * 
 * @return void
 */
void rxFHSSInterruptHandler(void);

/**
 * @brief Manges the frequency hopping and recieving of data, needs to be repeatedly called to hop frequencies.
 * 
 * @param radio SX1262 radio object connected to the SPI radio
 * @param data String buffer to hold recieved data, passed to radio.readData
 * 
 * @return integer which follows RadioLib status codes, an output of 1 will be returned if new data, otherwise 0
 */
int rxFHSS(SX1262& radio, String& data);

/**
 * @brief Manages the transmitting of data. Only a single packet's worth of data is able to be transmitted, this function checks with max_packet_size to ensure transmission isn't too long to fit in a packet
 * 
 * @param radio SX1262 radio object connected to the SPI radio
 * @param data String buffer holding data to be transmitted, passed to radio.
 * 
 * @return integer which follows RadioLib status codes (-4 is used to indicate the data is too big to transmit within a single packet)
 */
int txFHSS(SX1262& radio, String& data, uint8_t bytes);

/**
 * @brief Manages the transmitting of data. Only a single packet's worth of data is able to be transmitted, this function checks with max_size (provided argunemt) to ensure transmission isn't too long to fit in a packet
 * 
 * @param radio SX1262 radio object connected to the SPI radio
 * @param data String buffer holding data to be transmitted, passed to radio.
 * @param max_size Max size of transmission in bytes
 * 
 * @return integer which follows RadioLib status codes (-4 is used to indicate the data is too big to transmit within a single packet)
 */
int txFHSS(SX1262& radio, String data, uint8_t bytes, uint8_t max_size);

/**
 * @brief Function to determine the max size of a payload based on current radio settings and the max transmission time. This can be used to determine maxPacketSize and is recommended to only call this once every time the settings change (or on startup).
 * 
 * @param radio SX1262 radio object connected to the SPI radio, used to grab the current settings rather than having to pass the settings separately
 * @param max_packet_time The maximum amount of time allowed to be sending data in a single packet in ms
 * 
 * @return the max number of bytes which fit within the specified time given the current radio settings
 */
uint8_t maxBytes(SX1262& radio, unsigned long max_packet_time);

#endif
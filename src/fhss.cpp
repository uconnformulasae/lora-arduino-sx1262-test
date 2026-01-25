/*
 *  NAME: fhss.cpp
 *  Description: Includes the functions for fhss
 * 
 *  Author: Charlie K.
 *  Date: 1/25/26
 */
#include <fhss.h>

#include <Arduino.h>
#include <EEPROM.h>
#include <RadioLib.h>

const float fhss_freq_table[50] PROGMEM = {
    902.15f, 902.67f, 903.20f, 903.72f, 904.25f,
    904.77f, 905.30f, 905.82f, 906.35f, 906.87f,
    907.39f, 907.92f, 908.44f, 908.97f, 909.49f,
    910.02f, 910.54f, 911.07f, 911.59f, 912.12f,
    912.64f, 913.16f, 913.69f, 914.21f, 914.74f,
    915.26f, 915.79f, 916.31f, 916.84f, 917.36f,
    917.88f, 918.41f, 918.93f, 919.46f, 919.98f,
    920.51f, 921.03f, 921.56f, 922.08f, 922.61f,
    923.13f, 923.65f, 924.18f, 924.70f, 925.23f,
    925.75f, 926.28f, 926.80f, 927.33f, 927.85f
};
uint8_t fhss_index_map[50];
unsigned long base_time;
volatile bool rxFHSSFlag = false;
bool synced = false;
uint8_t fhss_current_index = 255;
uint8_t fhss_previous_index = 255;
unsigned long last_packet_time;
uint8_t max_packet_size = 0;

void initializeFHSS(SX1262& radio) {
    long seed;

    EEPROM.get(0, seed);

    initializeFHSS(radio, seed);
}

void initializeFHSS(SX1262& radio, long seed) {
    generateIndexMap(fhss_index_map, 50);
    randomizeIndexMap(fhss_index_map, 50, seed);
    radio.setPacketReceivedAction(rxFHSSInterruptHandler);
}

void generateIndexMap(uint8_t* map, uint8_t elements) {
    for (uint8_t i = 0; i < elements; i++) {
        map[i] = i; // Loop through elements and input the position as the value
    }
}

void randomizeIndexMap(uint8_t* map, uint8_t elements, long seed) {
    randomSeed(seed);

    for (uint8_t i = 0; i < elements - 1; i++) { // Shuffle indexes, elements - 1 because there is no element to switch with the last one
        uint8_t j = random(i + 1, elements);

        uint8_t tmp = map[i];
        map[i] = map[j];
        map[j] = tmp;

    }
}

float getFreqFHSS(uint8_t fhss_current_index) {
    return pgm_read_float(&fhss_freq_table[fhss_index_map[fhss_current_index]]);
}

void rxFHSSInterruptHandler(void) {
    rxFHSSFlag = true;
}

int rxFHSS(SX1262& radio, String& data) {
    int radio_status;

    if (synced && millis() - last_packet_time >=  max_sync_timeout) { // Previously synced but haven't recieved a packet in a while, then go to 'out of synced' status incase the tx and rx are out of sync
        synced = false;
    }

    if (synced) { // Radios are synced up! Perform the normal hopping math
        fhss_current_index = ((millis() - base_time) / max_hop_time) % 50;

    } else { // Radios are not synced, sit at freq index 0 until a packet is recieved which can be used to sync (This will cause the rx to delay by many seconds (like 20s) because of how long it takes for the tx to loop around)
        fhss_current_index = 0;
    }

    if (fhss_current_index != fhss_previous_index) { // Detect if we need to hop, if so, change freq and go back to recieving
        radio_status = radio.setFrequency(getFreqFHSS(fhss_current_index));
        if (radio_status != RADIOLIB_ERR_NONE) {return radio_status;}

        radio_status = radio.startReceive();
        if (radio_status != RADIOLIB_ERR_NONE) {return radio_status;}

        fhss_previous_index = fhss_current_index;
    }

    if (rxFHSSFlag) { // Packet recieved, do syncing math, read data into provided buffer
        unsigned long now = millis();
        last_packet_time = now; // Reset watchdog time

        unsigned long time_on_air = radio.getTimeOnAir(radio.getPacketLength()) / 1000;

        base_time = now - (fhss_previous_index * max_hop_time) - time_on_air;

        synced = true;

        radio_status = radio.readData(data);
        if (radio_status != RADIOLIB_ERR_NONE) {return radio_status;}

        radio_status = radio.startReceive();
        if (radio_status != RADIOLIB_ERR_NONE) {return radio_status;}

        rxFHSSFlag = false;

        return 1; // New data!
    }

    return RADIOLIB_ERR_NONE;
}

int txFHSS(SX1262& radio, String& data, uint8_t bytes) {
    return txFHSS(radio, data, bytes, max_packet_size);
}

int txFHSS(SX1262& radio, String data, uint8_t bytes, uint8_t max_size) {
    int radio_status;

    if (bytes > max_size) { return RADIOLIB_ERR_PACKET_TOO_LONG; } // Number of bytes is too big to send safely through a single packet, error out.

    unsigned long timeInSlot = (millis() - base_time) % max_hop_time;
    unsigned long timeLeft = max_hop_time - timeInSlot;
    unsigned long airTime = radio.getTimeOnAir(bytes) / 1000;

    // If the packet takes longer than the time left, wait.
    if (airTime + 40 > timeLeft) { // 40ms safety buffer
        delay(timeLeft + 5); // Wait until the next slot starts
    }

    fhss_current_index = ((millis() - base_time) / max_hop_time) % 50;

    if (fhss_current_index != fhss_previous_index) { // Detect if we need to hop, if so, change freq and go back to recieving
        radio_status = radio.setFrequency(getFreqFHSS(fhss_current_index));
        if (radio_status != RADIOLIB_ERR_NONE) {return radio_status;}

        fhss_previous_index = fhss_current_index;
        delay(10); // Gives the rx side a bit of extra time to catch up
    }

    return radio.transmit(data);
}

uint8_t maxBytes(SX1262& radio, unsigned long max_packet_time) {
    for (uint8_t bytes = 1; bytes < 255; bytes++) { 
        unsigned long airTime = radio.getTimeOnAir(bytes) / 1000; 
        
        if (airTime > max_packet_time) {
            // If even 1 byte is too long, return 0 (error condition)
            if (bytes == 1) return 0; 
            
            return bytes - 1; 
        }
    }
    return 255;
}
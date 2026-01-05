#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>

// --- I2C DEFINITIONS ---
// The NAU7802 and AS5600 both use the default I2C pins (GP4/SDA, GP5/SCL on Pico)

// AS5600 Encoder I2C Address (default)
#define AS5600_ADDR 0x36

// --- USB JOYSTICK SETUP ---
// Standard 10-bit resolution for joystick output (0 to 1023)
#define JOYSTICK_RESOLUTION 1024
#define JOYSTICK_MAX_VALUE (JOYSTICK_RESOLUTION - 1)

// --- THROTTLE CALIBRATION CONSTANTS (AS5600) ---
// AS5600 raw angle range (0 - 4095 for a 12-bit sensor)
#define AS5600_RAW_MAX 4095

// User-defined minimum and maximum **raw** AS5600 values for the 17-degree travel
// *** REPLACE THESE WITH YOUR CALIBRATED VALUES ***
// Min raw value when pedal is fully released
uint16_t throttle_min_raw = 1000;
// Max raw value when pedal is fully pressed
uint16_t throttle_max_raw = 2500;
// Flag to indicate if the throttle output needs to be inverted (usually true for AS5600)
const bool THROTTLE_INVERTED = true; 

// --- BRAKE CALIBRATION CONSTANTS (NAU7802) ---
// *** REPLACE THESE WITH YOUR CALIBRATED VALUES ***
// Load cell raw reading at no pressure (tare)
long brake_zero_raw = 50; // A small offset for stability
// Load cell raw reading at maximum desired braking pressure
long brake_max_raw = 100000;

// --- DEBUG SETTINGS ---
// #define DEBUG // Uncomment to enable Serial printing of values

#endif
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <stdint.h>


// AS5600 Encoder I2C Address (default)
#define AS5600_ADDR 0x36

// --- USB JOYSTICK SETUP ---
// Standard 10-bit resolution for joystick output (0 to 1023)
#define JOYSTICK_RESOLUTION 1024
#define JOYSTICK_MAX_VALUE (JOYSTICK_RESOLUTION - 1)

// --- THROTTLE CALIBRATION CONSTANTS (AS5600) ---
// AS5600 raw angle range (0 - 4095 for a 12-bit sensor)
#define AS5600_RAW_MAX 4095


// --- DEBUG SETTINGS ---
// #define DEBUG // Uncomment to enable Serial printing of values

#endif
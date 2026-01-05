#include "BrakeSensor.h"

// --- Constructor (Same) ---
BrakeSensor::BrakeSensor(Adafruit_NAU7802& chip) : nau7802_chip(chip) {
    zero_cal_raw = 50; 
    max_cal_raw = 100000;
}

// --- Initialization ---
bool BrakeSensor::begin() {
    if (!nau7802_chip.begin()) {
        return false;
    }
    
    // Configure NAU7802 using the library enums/constants
    nau7802_chip.setGain(NAU7802_GAIN_128);
    nau7802_chip.setRate(NAU7802_RATE_10SPS);
    nau7802_chip.setLDO(NAU7802_3V3);

    // The library does not expose an "enableSleep" method; ensure the device
    // is enabled via begin()/enable() if needed (begin() already does necessary init).

    // Perform an internal calibration
    nau7802_chip.calibrate(NAU7802_CALMOD_INTERNAL);

    return true;
}

// ... (rest of the file remains the same)
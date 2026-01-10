#include "BrakeSensor.h"
#ifndef CONFIGURATION_H
#include <Configuration.h>
#endif

// --- Constructor (Same) ---
BrakeSensor::BrakeSensor()
{
}

// --- Initialization ---
bool BrakeSensor::begin(long _zero_cal_raw, long _max_cal_raw)
{
    brake_zero_raw = _zero_cal_raw;
    brake_max_raw = _max_cal_raw;

    if (!nau7802.begin())
    {
        Serial.println("NAU7802 Not Found!");
        return false;
    }

    // Configure NAU7802 using the library enums/constants
    nau7802.setGain(NAU7802_GAIN_128);
    nau7802.setRate(NAU7802_RATE_10SPS);
    nau7802.setLDO(NAU7802_3V3);

    // Perform an internal calibration
    nau7802.calibrate(NAU7802_CALMOD_INTERNAL);
    Serial.println("NAU7802 Found!");

    return true;
}

int BrakeSensor::getMappedValue()
{
    long limited_brake_value = constrain(readRawValue(), brake_zero_raw, brake_max_raw);
    long mapped_brake_value = map(limited_brake_value, brake_zero_raw, brake_max_raw, 0, JOYSTICK_MAX_VALUE);
    return constrain(mapped_brake_value, 0, JOYSTICK_MAX_VALUE);
}

long BrakeSensor::readRawValue()
{
    long rawValue = 0;
    if (nau7802.available())
    {
        rawValue = nau7802.read();
    }
    Serial.print("Brake Raw Value: ");
    Serial.println(rawValue);
    return rawValue;
}

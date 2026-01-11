#include "BrakeSensor.h"
#ifndef CONFIGURATION_H
#include <Configuration.h>
#endif

// --- Constructor (Same) ---
BrakeSensor::BrakeSensor()
{
}

/**
 * @brief Initialize the Brake Sensor (NAU7802 Load Cell Amplifier)
 * @param _brake_zero_raw The raw load cell reading at zero braking pressure
 * @param _brake_max_raw The raw load cell reading at maximum braking pressure
 * @param _lowerDeadzone The lower deadzone threshold for the brake as a percentage of
 * full scale (This allows you to rest your foot against the pedal without any braking input)
 * @param _upperDeadzone The upper deadzone threshold for the brake as a percentage of
 * full scale (This allows you to more consistenly hit maximum braking pressure)
 * @return true if initialization was successful, false otherwise
 */
bool BrakeSensor::begin(long _brake_max_raw, float_t _lowerDeadzone, float_t _upperDeadzone)
{
    brake_max_raw = _brake_max_raw;
    lowerDeadzone = _lowerDeadzone;
    upperDeadzone = _upperDeadzone;

    if (!nau7802.begin())
    {
        Serial.println("NAU7802 Not Found!");
        return false;
    }

    // Configure NAU7802 using the library enums/constants
    nau7802.setGain(NAU7802_GAIN_128);
    nau7802.setRate(NAU7802_RATE_320SPS); // Sample at 320 samples per second
    nau7802.setLDO(NAU7802_3V3);

    // Perform an internal calibration
    nau7802.calibrate(NAU7802_CALMOD_INTERNAL);
    Serial.println("NAU7802 Found!");

    return true;
}

long BrakeSensor::getMappedValue()
{
    long raw_brake_value = readRawValue();
#ifdef DEBUG
    Serial.print("Raw Brake Value: ");
    Serial.println(raw_brake_value);
#endif
    // Apply deadzone
    long brake_range = brake_max_raw - brake_zero_raw;
    long lower_deadzone_threshold = brake_zero_raw + static_cast<long>(brake_range * lowerDeadzone);
    long upper_deadzone_threshold = brake_max_raw - static_cast<long>(brake_range * upperDeadzone);

    if (raw_brake_value < lower_deadzone_threshold)
    {
        raw_brake_value = brake_zero_raw;
    }
    else if (raw_brake_value > upper_deadzone_threshold)
    {
        raw_brake_value = brake_max_raw;
    }

    long mapped_brake_value = mapFloat(raw_brake_value, brake_zero_raw, brake_max_raw, 0, JOYSTICK_MAX_VALUE);
#ifdef DEBUG
    Serial.print("Mapped Brake Value: ");
    Serial.println(mapped_brake_value);
#endif
    return constrain(mapped_brake_value, 0, JOYSTICK_MAX_VALUE);
}

long BrakeSensor::readRawValue()
{
    long rawValue = 0;
    if (nau7802.available())
    {
        rawValue = nau7802.read();
    }
    else
    {
        rawValue = previousRawBrakeValue;
    }

    previousRawBrakeValue = rawValue;

    return rawValue;
}
void BrakeSensor::calibrate()
{  
    long sumReadings = 0;
     // Set the current reading as the zero brake value
    for (int i = 0; i < 5; i++)
    {
        delay(50);
        // Read multiple times to stabilize
        sumReadings += readRawValue();
     }
    brake_zero_raw = sumReadings / 5;
    Serial.print("Brake Calibrated. New Zero Raw Value: ");
    Serial.println(brake_zero_raw);

}


float BrakeSensor::mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
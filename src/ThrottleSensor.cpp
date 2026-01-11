#include "ThrottleSensor.h"
#ifndef CONFIGURATION_H
#include <Configuration.h>
#endif

// --- Constructor ---
ThrottleSensor::ThrottleSensor()
{
}

/** @brief Initialize the Throttle Sensor (AS5600 Rotary Encoder)
 * @param _throttle_min_raw The raw angle reading when the pedal is fully released
 * @param _throttle_max_raw The raw angle reading when the pedal is fully pressed
 * @param _is_inverted Whether the throttle axis is inverted
 * @param _deadzone The deadzone threshold for bottom and top of pedal travel
 * @return true if initialization was successful, false otherwise
 */
bool ThrottleSensor::begin(uint16_t _throttle_min_raw, uint16_t _throttle_max_raw, uint16_t _deadzone)
{
    if (!as5600.begin())
    {
        Serial.println("AS5600 Not Found!");
        return false;
    }

    throttle_min_raw = _throttle_min_raw;
    throttle_max_raw = _throttle_max_raw;
    deadzone = _deadzone;
    Serial.println("AS5600 Found!");
    return true;
}

// --- Calibration Setter ---
void ThrottleSensor::setCalibration(uint16_t min_raw, uint16_t max_raw)
{
    throttle_min_raw = min_raw;
    throttle_max_raw = max_raw;
}
// --- Raw Sensor Read ---
uint16_t ThrottleSensor::readRawAngle()
{
    uint16_t rawAngle = as5600.readAngle();
    return rawAngle;
}

// --- Mapped Value Getter ---
long ThrottleSensor::getMappedValue()
{
    uint16_t raw_angle = readRawAngle();

    // Handle wrap-around for raw_angle
    if (throttle_min_raw < throttle_max_raw)
    {
        raw_angle += AS5600_RAW_MAX;
    }

    // Deadzone handling: shrink the active range
    // Again raw_max is smaller than the raw_min due to mechanical setup
    int32_t active_max = throttle_max_raw + deadzone;
    int32_t active_min = throttle_min_raw - deadzone;

    long mapped_value;
    if (raw_angle >= active_min)
    {
        mapped_value = 0;
    }
    else if (raw_angle <= active_max)
    {
        mapped_value = JOYSTICK_RESOLUTION;
    }
    else
    {
        // Linear mapping, inverted for reversed direction
        mapped_value = JOYSTICK_RESOLUTION - ((long)(raw_angle - active_max) * JOYSTICK_RESOLUTION / (active_min - active_max));
    }
    return constrain(mapped_value, 0, JOYSTICK_RESOLUTION);
}
#include "ThrottleSensor.h"
#ifndef CONFIGURATION_H
#include <Configuration.h>
#endif

// --- Constructor ---
ThrottleSensor::ThrottleSensor()
{
}

// --- Initialization ---
bool ThrottleSensor::begin(uint16_t _min_cal_raw, uint16_t _max_cal_raw, bool _is_inverted)
{
    if (!as5600.begin())
    {
        Serial.println("AS5600 Not Found!");
        return false;
    }

    min_cal_raw = _min_cal_raw;
    max_cal_raw = _max_cal_raw;
    is_inverted = _is_inverted;
    Serial.println("AS5600 Found!");
    return true;
}

// --- Calibration Setter ---
void ThrottleSensor::setCalibration(uint16_t min_raw, uint16_t max_raw, bool inverted)
{
    min_cal_raw = min_raw;
    max_cal_raw = max_raw;
    is_inverted = inverted;
}

// --- Raw Sensor Read ---
uint16_t ThrottleSensor::readRawAngle()
{
    uint16_t rawAngle = as5600.readAngle();
    Serial.print("Throttle Raw Angle: ");
    Serial.println(rawAngle);
    return rawAngle;
}

// --- Mapped Value Getter ---
int ThrottleSensor::getMappedValue()
{
    uint16_t raw_angle = readRawAngle();

    uint16_t limited_angle = constrain(raw_angle, min_cal_raw, max_cal_raw);

    long mapped_value = map(
        limited_angle,
        min_cal_raw,
        max_cal_raw,
        0,
        JOYSTICK_RESOLUTION);

    if (is_inverted)
    {
        mapped_value = JOYSTICK_RESOLUTION - mapped_value;
    }

    return constrain(mapped_value, 0, JOYSTICK_RESOLUTION);
}
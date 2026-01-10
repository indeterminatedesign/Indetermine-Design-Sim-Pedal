#include "ThrottleSensor.h"
#ifndef CONFIGURATION_H
#include <Configuration.h>
#endif

// --- Constructor ---
ThrottleSensor::ThrottleSensor()
{
}

// --- Initialization ---
void ThrottleSensor::begin(uint16_t _min_cal_raw, uint16_t _max_cal_raw, bool _is_inverted)
{
    min_cal_raw = _min_cal_raw;
    max_cal_raw = _max_cal_raw;
    is_inverted = _is_inverted;
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
    uint16_t rawAngle = 0;
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(0x0C); // RAW_ANGLE_H register address
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    if (Wire.available() == 2)
    {
        rawAngle = Wire.read() << 8;
        rawAngle |= Wire.read();
    }
    Serial.print("Throttle Raw Angle: ");
    Serial.println(rawAngle);
    return rawAngle & AS5600_RAW_MAX;
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
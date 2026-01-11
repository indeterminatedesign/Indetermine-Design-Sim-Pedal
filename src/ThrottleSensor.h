#ifndef THROTTLE_SENSOR_H
#define THROTTLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <AS5600.h>

#define AS5600_ADDR 0x36
#define AS5600_RAW_MAX 4095

class ThrottleSensor
{
public:
    ThrottleSensor();
    bool begin(uint16_t _throttle_min_raw, uint16_t _throttle_max_raw, uint16_t _deadzone);
    void setCalibration(uint16_t min_raw, uint16_t max_raw);
    long getMappedValue();
    uint16_t readRawAngle();

private:
    // Min raw value when pedal is fully released
    uint16_t throttle_min_raw;
    // Max raw value when pedal is fully pressed
    uint16_t throttle_max_raw;
    //Deadzone threshold for bottom and top of pedal travel (For example the total range is 0-4095, a deadzone of 10 would be about 0.24%, meaning a reading of 4085-4095 is 100% throttle)
    uint16_t deadzone = 10;

    AS5600 as5600;
};

#endif
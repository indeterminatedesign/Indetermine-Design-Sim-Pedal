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
    bool begin(uint16_t _min_cal_raw, uint16_t _max_cal_raw, bool _is_inverted);
    void setCalibration(uint16_t min_raw, uint16_t max_raw, bool inverted);
    int getMappedValue();
    uint16_t readRawAngle();

private:
    uint16_t min_cal_raw;
    uint16_t max_cal_raw;
    // Min raw value when pedal is fully released
    uint16_t throttle_min_raw;
    // Max raw value when pedal is fully pressed
    uint16_t throttle_max_raw;
    bool is_inverted = false;

    AS5600 as5600;
};

#endif
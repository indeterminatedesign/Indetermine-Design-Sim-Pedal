#ifndef THROTTLE_SENSOR_H
#define THROTTLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

#define AS5600_ADDR 0x36
#define AS5600_RAW_MAX 4095

class ThrottleSensor
{
public:
    ThrottleSensor();
    void begin(uint16_t _min_cal_raw, uint16_t _max_cal_raw, bool _is_inverted);
    void setCalibration(uint16_t min_raw, uint16_t max_raw, bool inverted);
    int getMappedValue();

private:
    uint16_t min_cal_raw;
    uint16_t max_cal_raw;
     // Min raw value when pedal is fully released
    uint16_t throttle_min_raw;
    // Max raw value when pedal is fully pressed
    uint16_t throttle_max_raw;
    bool is_inverted = false;
    uint16_t readRawAngle();
};

#endif
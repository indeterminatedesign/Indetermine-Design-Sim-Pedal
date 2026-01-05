#ifndef THROTTLE_SENSOR_H
#define THROTTLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

#define AS5600_ADDR 0x36
#define AS5600_RAW_MAX 4095

class ThrottleSensor {
public:
    ThrottleSensor();
    void begin();
    void setCalibration(uint16_t min_raw, uint16_t max_raw, bool inverted);
    int getMappedValue(int max_output_value);

private:
    uint16_t min_cal_raw;
    uint16_t max_cal_raw;
    bool is_inverted;
    uint16_t readRawAngle();
};

#endif
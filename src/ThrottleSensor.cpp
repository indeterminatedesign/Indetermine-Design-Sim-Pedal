#include "ThrottleSensor.h"

// --- Constructor ---
ThrottleSensor::ThrottleSensor() {
    min_cal_raw = 1000;
    max_cal_raw = 2500;
    is_inverted = true;
}

// --- Initialization ---
void ThrottleSensor::begin() {
    // I2C (Wire.begin()) is handled in main.cpp's setup()
}

// --- Calibration Setter ---
void ThrottleSensor::setCalibration(uint16_t min_raw, uint16_t max_raw, bool inverted) {
    min_cal_raw = min_raw;
    max_cal_raw = max_raw;
    is_inverted = inverted;
}

// --- Raw Sensor Read ---
uint16_t ThrottleSensor::readRawAngle() {
    uint16_t rawAngle = 0;
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(0x0C); // RAW_ANGLE_H register address
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    if (Wire.available() == 2) {
        rawAngle = Wire.read() << 8;
        rawAngle |= Wire.read();
    }
    return rawAngle & AS5600_RAW_MAX;
}

// --- Mapped Value Getter ---
int ThrottleSensor::getMappedValue(int max_output_value) {
    uint16_t raw_angle = readRawAngle();

    uint16_t limited_angle = constrain(raw_angle, min_cal_raw, max_cal_raw);

    long mapped_value = map(
        limited_angle,
        min_cal_raw,
        max_cal_raw,
        0,
        max_output_value
    );

    if (is_inverted) {
        mapped_value = max_output_value - mapped_value;
    }

    return constrain(mapped_value, 0, max_output_value);
}
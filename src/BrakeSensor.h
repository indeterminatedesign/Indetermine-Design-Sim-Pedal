#ifndef BRAKE_SENSOR_H
#define BRAKE_SENSOR_H

#include <Arduino.h>
#include <Adafruit_NAU7802.h> 

class BrakeSensor {
public:
    BrakeSensor(Adafruit_NAU7802& chip);

    bool begin();

    void setCalibration(long zero_raw, long max_raw);

    int getMappedValue(int max_output_value);

    // REMOVED: void tare() { nau7802_chip.tare(); } // NO LONGER EXISTS

private:
    Adafruit_NAU7802& nau7802_chip;
    long zero_cal_raw;
    long max_cal_raw;

    // Read the raw value from the NAU7802 (helper)
    long readRawValue();
};

#endif
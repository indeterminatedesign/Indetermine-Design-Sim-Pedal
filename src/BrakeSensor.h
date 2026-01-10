#ifndef BRAKE_SENSOR_H
#define BRAKE_SENSOR_H

#include <Arduino.h>
#include <Adafruit_NAU7802.h>

class BrakeSensor
{
public:
  BrakeSensor();

  bool begin(long _zero_cal_raw, long _max_cal_raw);

  void setCalibration(long zero_raw, long max_raw);

  int getMappedValue();

private:
  Adafruit_NAU7802 nau7802;
  long zero_cal_raw;
  long max_cal_raw;
  long brake_zero_raw;
  // Load cell raw reading at maximum desired braking pressure
  long brake_max_raw;
  // Read the raw value from the NAU7802 (helper)
  long readRawValue();
};
#endif
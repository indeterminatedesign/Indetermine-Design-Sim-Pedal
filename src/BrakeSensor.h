#ifndef BRAKE_SENSOR_H
#define BRAKE_SENSOR_H

#include <Arduino.h>
#include <Adafruit_NAU7802.h>

class BrakeSensor
{
public:
  BrakeSensor();

  bool begin(long _brake_max_raw, float_t _lowerDeadzone, float_t _upperDeadzone);

  void calibrate();

  long getMappedValue();
  long readRawValue();

private:
  Adafruit_NAU7802 nau7802;
  // Load cell raw reading at zero braking pressure
  long brake_zero_raw;
  // Load cell raw reading at maximum braking pressure
  long brake_max_raw;
  // This is the lower deadzone threshold for the brake as a percentage of full scale  (This allows you to rest your foot against the pedal without any braking input)
  float_t lowerDeadzone;
  // This is the upper deadzone threshold for the brake as a percentage of full scale (This allows you to more consistenly hit maximum braking pressure)
  float_t upperDeadzone;

  //The NAU7802 returns a 0 if a new reading is not available, so we need to track the previous valid reading
  float_t previousRawBrakeValue = 0;

  long emaFilter(float_t alpha, long currentValue, long previousValue);
  float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};
#endif
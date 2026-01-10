// Clean main file used for building while original src/main.cpp is being repaired
// --- LIBRARIES ---
#include <Wire.h>
//#include <Configuration.h>
#include <BrakeSensor.h>
#include <ThrottleSensor.h>
#ifndef USE_JOYSTICK
#define USE_JOYSTICK 1
#endif

#if USE_JOYSTICK
#if defined(USB_JOYSTICK)
#include <usb_joystick.h>
#endif
#endif

BrakeSensor brakeSensor;
ThrottleSensor throttleSensor;

int readBrakeValue();
int readThrottleValue();

void setup()
{
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Wire.setClock(400000); // 400kHz I2C
  Wire.begin();

  brakeSensor.begin(0,100000);
  throttleSensor.begin(0,4095,false);

  // Load Min and Max values from EEPROM and set the calibrations for both sensors

  Serial.println("Sim Pedal Device Initialized (clean main)");
}

// --- MAIN LOOP ---
void loop()
{
  int brake_output = readBrakeValue();
  int throttle_output = readThrottleValue();

  static uint32_t lastBlink = 0;
  static bool ledState = false;
  if (millis() - lastBlink >= 500)
  {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(13, ledState ? HIGH : LOW);
  }

#if USE_JOYSTICK
  Joystick.X(brake_output);
  Joystick.Y(throttle_output);
  Joystick.Z(0);
  Joystick.Zrotate(0);
  Joystick.sliderLeft(0);
  Joystick.sliderRight(0);
  Joystick.hat(-1);
  Joystick.button(1, 0);
  Joystick.button(2, 0);
  Joystick.send_now();
#else
  Serial.print("Brake:");
  Serial.print(brake_output);
  Serial.print(" | Throttle:");
  Serial.println(throttle_output);
#endif

  delay(10);
}

int readBrakeValue()
{
return brakeSensor.getMappedValue();
}

int readThrottleValue()
{
return throttleSensor.getMappedValue();
}
#include <Wire.h>
#include <BrakeSensor.h>
#include <ThrottleSensor.h>

BrakeSensor brakeSensor;
ThrottleSensor throttleSensor;

int readBrakeValue();
int readThrottleValue();

void setup()
{
  Serial.begin(115200);

  Wire.setClock(400000); // 400kHz I2C
  Wire.begin();

  delay(10000);
  brakeSensor.begin(500000, .02, .05);
  brakeSensor.calibrate();
  throttleSensor.begin(3948, 3762, 10);

  // Load Min and Max values from EEPROM and set the calibrations for both sensors

  Serial.println("Sim Pedal Device Initialized");
}

// --- MAIN LOOP ---
void loop()
{
  Serial.print("RAW READINGS:");
  Serial.print(throttleSensor.readRawAngle());
  Serial.print(",");
  Serial.println(brakeSensor.readRawValue());
  Serial.print("MAPPED VALUES:");
  Serial.print(throttleSensor.getMappedValue());
  Serial.print(",");
  Serial.println(brakeSensor.getMappedValue());

  Joystick.X(throttleSensor.getMappedValue());
  Joystick.Y(brakeSensor.getMappedValue());
  Joystick.Z(0);
  Joystick.Zrotate(0);
  Joystick.button(1, 1);
  Joystick.button(2, 1);
  delay(100);

  /*
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
  */
}

int readBrakeValue()
{
  return brakeSensor.getMappedValue();
}

int readThrottleValue()
{
  return throttleSensor.getMappedValue();
}
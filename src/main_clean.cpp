// Clean main file used for building while original src/main.cpp is being repaired
// --- LIBRARIES ---
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#ifndef USE_JOYSTICK
#define USE_JOYSTICK 0
#endif

#if USE_JOYSTICK
#include "Adafruit_TinyUSB.h"
#endif

#include "Configuration.h"

Adafruit_NAU7802 nau7802 = Adafruit_NAU7802();

#if USE_JOYSTICK
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_GAMEPAD() };
Adafruit_USBD_HID usb_hid;
hid_gamepad_report_t gp;
#endif

int readBrakeValue(Adafruit_NAU7802& nau7802);
int readThrottleValue();

void setup() {
  Serial.begin(115200);
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
  Wire.begin();

  if (!nau7802.begin()) {
    Serial.println("NAU7802 not found! Check wiring and I2C.");
    while (1);
  }
  nau7802.setGain(NAU7802_GAIN_128);
  nau7802.setRate(NAU7802_RATE_10SPS);
  nau7802.setLDO(NAU7802_3V3);
  nau7802.calibrate(NAU7802_CALMOD_INTERNAL);

#if USE_JOYSTICK
  TinyUSBDevice.setManufacturerDescriptor("Indetermine");
  TinyUSBDevice.setProductDescriptor("Indetermine Sim Pedal");
  TinyUSBDevice.setSerialDescriptor("SIMPEDAL-001");
  TinyUSBDevice.setID(0x1209, 0x0001);
  if (!TinyUSBDevice.isInitialized()) TinyUSBDevice.begin(0);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.begin();
#endif

  Serial.println("Sim Pedal Device Initialized (clean main)");
}

void loop() {
  int brake_output = readBrakeValue(nau7802);
  int throttle_output = readThrottleValue();

  static uint32_t lastBlink = 0;
  static bool ledState = false;
  if (millis() - lastBlink >= 500) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(25, ledState ? HIGH : LOW);
  }

#if USE_JOYSTICK
  if (TinyUSBDevice.mounted() && usb_hid.ready()) {
    gp.x = (int8_t)map(brake_output, 0, JOYSTICK_MAX_VALUE, -127, 127);
    gp.y = (int8_t)map(throttle_output, 0, JOYSTICK_MAX_VALUE, -127, 127);
    gp.z = 0; gp.rz = 0; gp.rx = 0; gp.ry = 0; gp.hat = 0; gp.buttons = 0;
    usb_hid.sendReport(0, &gp, sizeof(gp));
  } else {
    Serial.print("Brake:"); Serial.print(brake_output);
    Serial.print(" | Throttle:"); Serial.println(throttle_output);
  }
#else
  Serial.print("Brake:"); Serial.print(brake_output);
  Serial.print(" | Throttle:"); Serial.println(throttle_output);
#endif

  delay(10);
}

uint16_t readAS5600RawAngle() {
  uint16_t rawAngle = 0;
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0C);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 2);
  if (Wire.available() == 2) {
    rawAngle = Wire.read() << 8;
    rawAngle |= Wire.read();
  }
  return rawAngle & AS5600_RAW_MAX;
}

int readBrakeValue(Adafruit_NAU7802& nau7802) {
  long raw_brake_value = 0;
  if (nau7802.available()) raw_brake_value = nau7802.read();
  long limited_brake_value = constrain(raw_brake_value, brake_zero_raw, brake_max_raw);
  long mapped_brake_value = map(limited_brake_value, brake_zero_raw, brake_max_raw, 0, JOYSTICK_MAX_VALUE);
  return constrain(mapped_brake_value, 0, JOYSTICK_MAX_VALUE);
}

int readThrottleValue() {
  uint16_t raw_angle = readAS5600RawAngle();
  uint16_t limited_angle = constrain(raw_angle, throttle_min_raw, throttle_max_raw);
  long mapped_throttle_value = map(limited_angle, throttle_min_raw, throttle_max_raw, 0, JOYSTICK_MAX_VALUE);
  if (THROTTLE_INVERTED) mapped_throttle_value = JOYSTICK_MAX_VALUE - mapped_throttle_value;
  return constrain(mapped_throttle_value, 0, JOYSTICK_MAX_VALUE);
}

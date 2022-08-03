#pragma once

#include <Arduino.h>

#include "Sensor.h"

class Adafruit_ADS1X15;
class AnalogADS1115PressureSensor : public Sensor {
 public:
  AnalogADS1115PressureSensor(const String &id, SensorAction *sensorAction,
                              uint8_t channel, float minPressure,
                              float maxPressure);

 protected:
  virtual void readAndInform();
  virtual void discovery();

  uint8_t m_channel;
  float m_minPressure;
  float m_maxPressure;
  Adafruit_ADS1X15 *m_ads;
};

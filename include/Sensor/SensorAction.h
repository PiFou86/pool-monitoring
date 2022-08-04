#pragma once
#include <Arduino.h>

class SensorAction {
 public:
  virtual void dataRead(const String &id, const String &device_type,
                        const String &unit, const float &value) = 0;
  virtual void discovery(const String &id, const String &device_type,
                         const String &unit) = 0;
};

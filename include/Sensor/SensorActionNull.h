#pragma once
#include <Arduino.h>

#include "SensorAction.h"

class SensorActionNull : public SensorAction {
 public:
  inline virtual void dataRead(const String &id, const String &device_type,
                               const String &unit,
                               const float &value) override {
    ;
  };
  inline virtual void discovery(const String &id, const String &device_type,
                                const String &unit) override {
    ;
  }
};
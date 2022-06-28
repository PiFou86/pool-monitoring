#pragma once
#include <Arduino.h>
#include "SensorAction.h"

class SensorActionNull : public SensorAction {
public:
    inline virtual void dataRead(const String &id, const String &dataName, const String &unit, const float &value) override { ; };
};
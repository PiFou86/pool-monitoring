#pragma once
#include <Arduino.h>

#include "SensorAction.h"

class SensorAction;
class Sensor {
public:
    Sensor(const String &id, SensorAction *sensorAction);

    virtual void readAndInform() = 0;

protected:
    inline void dataRead(const String &dataName, const float &value, const String &unit) { this->m_sensorAction->dataRead(this->m_id, dataName, unit, value); }

private:
    String m_id;
    SensorAction *m_sensorAction;
};

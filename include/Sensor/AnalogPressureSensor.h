#pragma once

#include <Arduino.h>

#include "Sensor.h"

class AnalogPressureSensor : public Sensor {
public:
    AnalogPressureSensor(const String &id, SensorAction *sensorAction, uint8_t pin, float minPressure, float maxPressure);
    virtual void readAndInform();

private:
    uint8_t m_pin;
    
    float m_minPressure;
    float m_maxPressure;
};

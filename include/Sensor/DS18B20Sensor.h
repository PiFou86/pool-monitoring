#pragma once
#include <Arduino.h>

#include <DallasTemperature.h>

#include "Sensor/Sensor.h"

class DS18B20Sensor : public Sensor {
public:
    DS18B20Sensor(const String &id, SensorAction *sensorAction, OneWire *oneWire, const uint8_t* sensorAddress);
    virtual void readAndInform();

private:
    DallasTemperature *m_ds18b20;
    const uint8_t* m_sensorAddress;
};

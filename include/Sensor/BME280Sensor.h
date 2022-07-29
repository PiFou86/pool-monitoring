#pragma once
#include "Arduino.h"
#include "Sensor.h"


class Adafruit_BME280;

class BME280Sensor : public Sensor {
public:
    BME280Sensor(const String &id, SensorAction *sensorAction);

protected:
    virtual void readAndInform();

private:
    Adafruit_BME280 *m_bme280;
};

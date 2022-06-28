#include <Arduino.h>
#include <Adafruit_BME280.h>

#include "Log/Logger.h"
#include "Sensor/BME280Sensor.h"

BME280Sensor::BME280Sensor(const String &id, SensorAction *sensorAction) : Sensor(id, sensorAction), m_bme280(nullptr)
{
    Adafruit_BME280 *bme280 = new Adafruit_BME280();

    bool status = bme280->begin();
    if (!status) {
        Logger.errorln("BME280 not detected !");
    } else {
        this->m_bme280 = bme280;
    }
}

void BME280Sensor::readAndInform() {
    this->dataRead("temperature", this->m_bme280->readTemperature(), "°C");
    this->dataRead("pressure", this->m_bme280->readPressure(), "hPa");
    this->dataRead("humidity", this->m_bme280->readHumidity(), "%");
}
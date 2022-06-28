#include "Sensor/DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(const String &id, SensorAction *sensorAction, OneWire *oneWire, const uint8_t *sensorAddress) 
: Sensor(id, sensorAction), m_ds18b20(nullptr), m_sensorAddress(sensorAddress)
{
    DallasTemperature *ds18b20 = new DallasTemperature(oneWire);
    ds18b20->begin();

    this->m_ds18b20 = ds18b20;
}

void DS18B20Sensor::readAndInform() {
    this->dataRead("temperature", this->m_ds18b20->getTempC(this->m_sensorAddress), "°C");
}

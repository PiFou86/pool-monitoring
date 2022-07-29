#include "Sensor/DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(const String &id, SensorAction *sensorAction, OneWire *oneWire, const uint8_t *sensorAddress) 
: Sensor(id, sensorAction), m_ds18b20(nullptr), m_sensorAddress(sensorAddress)
{
    DallasTemperature *ds18b20 = new DallasTemperature(oneWire);
    ds18b20->begin();

    ds18b20->requestTemperaturesByAddress(this->m_sensorAddress);
    float temperature = ds18b20->getTempC(this->m_sensorAddress);

    if (abs(85 - temperature) > 0.01) {
        this->m_ds18b20 = ds18b20;
        this->ready();
    }
}

void DS18B20Sensor::readAndInform() {
    this->m_ds18b20->requestTemperatures();
    this->dataRead("temperature", this->m_ds18b20->getTempC(this->m_sensorAddress), "°C");
}

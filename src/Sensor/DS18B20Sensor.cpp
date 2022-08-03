#include "Sensor/DS18B20Sensor.h"

#include "Configuration/Configuration.h"

DS18B20Sensor::DS18B20Sensor(const String &id, SensorAction *sensorAction, OneWire *oneWire, const uint8_t *sensorAddress) 
: Sensor(id, sensorAction), m_ds18b20(nullptr), m_sensorAddress(sensorAddress)
{
    DallasTemperature *ds18b20 = new DallasTemperature(oneWire);
    ds18b20->begin();

    ds18b20->requestTemperaturesByAddress(this->m_sensorAddress);
    float temperature = ds18b20->getTempC(this->m_sensorAddress);

    if (abs(85 - temperature) > 0.01) {
        Logger.infoln(String(F("DS18B20 used with address: ")) + Configuration.addressToString(this->m_sensorAddress, 8));
        this->m_ds18b20 = ds18b20;
        this->ready();
    }
}

void DS18B20Sensor::readAndInform() {
    this->m_ds18b20->requestTemperatures();
    this->dataRead(F("temperature"), this->m_ds18b20->getTempC(this->m_sensorAddress), F("°C"));
}

void DS18B20Sensor::discovery() {
  this->actionDiscovery(String(F("temperature")), this->getId(), String(F("°C")));
}

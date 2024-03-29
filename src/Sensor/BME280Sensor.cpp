#include "Sensor/BME280Sensor.h"

#include <Adafruit_BME280.h>
#include <Arduino.h>

#include "Log/Logger.h"

BME280Sensor::BME280Sensor(const String &id, SensorAction *sensorAction)
    : Sensor(id, sensorAction), m_bme280(nullptr) {
  Adafruit_BME280 *bme280 = new Adafruit_BME280();

  bool status = bme280->begin(0x76);
  if (!status) {
    Logger.errorln(F("BME280 not detected !"));
  } else {
    Logger.infoln(F("BME280  detected !"));
    this->m_bme280 = bme280;
    this->ready();
  }
}

void BME280Sensor::readAndInform() {
  this->dataRead(F("temperature"), this->m_bme280->readTemperature(), F("°C"));
  this->dataRead(F("pressure"), this->m_bme280->readPressure() / 100.0f, F("hPa"));
  this->dataRead(F("humidity"), this->m_bme280->readHumidity(), F("%"));
}

void BME280Sensor::discovery() {
  this->actionDiscovery(String(F("temperature")), String(F("°C")));
  this->actionDiscovery(String(F("pressure")), String(F("hPa")));
  this->actionDiscovery(String(F("humidity")), String(F("%")));
}

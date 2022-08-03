#include "Sensor/AnalogADS1115PressureSensor.h"

#include <Adafruit_ADS1X15.h>

AnalogADS1115PressureSensor::AnalogADS1115PressureSensor(
    const String &id, SensorAction *sensorAction, uint8_t channel,
    float minPressure, float maxPressure)
    : Sensor(id, sensorAction),
      m_channel(channel),
      m_minPressure(minPressure),
      m_maxPressure(maxPressure),
      m_ads(nullptr) {
  Adafruit_ADS1X15 *ads = new Adafruit_ADS1X15();
  if (ads->begin()) {
    Logger.infoln(F("ADS1115 found"));
    ads->setGain(GAIN_TWO);
    Logger.verboseln(F("ADS1115 gain set to 2 (2,048v)"));
    this->m_ads = ads;
    this->ready();
  } else {
    Logger.errorln(F("ADS1115 not found"));
  }
}

void AnalogADS1115PressureSensor::readAndInform() {
  float ADSvoltage =
      this->m_ads->readADC_SingleEnded(this->m_channel) * 0.0625 / 1000.0;
  Logger.verboseln(String(F("ADS1115 ADS reading voltage: ")) + String(ADSvoltage) +
                String(F("V / 2.048 V")));
  float voltage = ADSvoltage / 2.048 * 5.0;
  Logger.verboseln(String(F("ADS1115 voltage: ")) + String(voltage) +
                String(F("V / 5 V")));
  float pressure = voltage / 4.5 * (this->m_maxPressure - this->m_minPressure) +
                   this->m_minPressure;

  this->dataRead(F("pressure"), pressure, F("PSI"));
}

void AnalogADS1115PressureSensor::discovery() {
  this->actionDiscovery(String(F("pressure")), this->getId(), String(F("PSI")));
}

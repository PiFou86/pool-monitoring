#pragma once
#include <Arduino.h>

#include "Log/Logger.h"
#include "Readiness.h"
#include "SensorAction.h"

class SensorAction;
class Sensor : public Readiness {
 public:
  Sensor(const String &id, SensorAction *sensorAction);

  inline void process() {
    if (this->isReady()) {
      this->readAndInform();
    } else {
      Logger.infoln("Sensor is not ready (" + this->m_id + ")");
    }
  }

  inline bool isReady() const { return this->m_isReady; }
  inline void ready() { this->m_isReady = true; }
  inline void notReady() { this->m_isReady = false; }

 protected:
  inline void dataRead(const String &dataName, const float &value,
                       const String &unit) {
    this->m_sensorAction->dataRead(this->m_id, dataName, unit, value);
  }
  virtual void readAndInform() = 0;

 private:
  String m_id;
  SensorAction *m_sensorAction;
  bool m_isReady;
};

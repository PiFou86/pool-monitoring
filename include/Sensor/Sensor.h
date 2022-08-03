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
      Logger.infoln(String(F("Sensor is not ready (")) + this->m_id +
                    String(F(")")));
    }
  }

  inline void sendAutodiscovery() {
    if (this->isReady()) {
      this->discovery();
    } else {
      Logger.infoln(String(F("Sensor is not ready (")) + this->m_id +
                    String(F(")")));
    }
  }

  inline bool isReady() const { return this->m_isReady; }
  inline void ready() { this->m_isReady = true; }
  inline void notReady() { this->m_isReady = false; }

  inline const String &getId() const { return this->m_id; }

 protected:
  inline void dataRead(const String &dataName, const float &value,
                       const String &unit) {
    this->m_sensorAction->dataRead(this->m_id, dataName, unit, value);
  }
  inline void actionDiscovery(const String &device_type, const String &name,
                              const String &unit) const {
    this->m_sensorAction->discovery(this->m_id, device_type, name, unit);
  }
  virtual void readAndInform() = 0;
  virtual void discovery() = 0;

 private:
  String m_id;
  SensorAction *m_sensorAction;
  bool m_isReady;
};

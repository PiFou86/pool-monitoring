#include <vector>

#include "SensorAction.h"

class SensorActionAggregator : public SensorAction {
 private:
  std::vector<SensorAction *> m_sensorActions;

 public:
  inline void addSensorAction(SensorAction *sensorAction) {
    this->m_sensorActions.push_back(sensorAction);
  };
  inline virtual void dataRead(const String &id, const String &device_type,
                               const String &unit,
                               const float &value) override {
    for (SensorAction *sensorAction : this->m_sensorActions) {
      sensorAction->dataRead(id, device_type, unit, value);
    }
  }
  inline virtual void discovery(const String &id, const String &device_type,
                                const String &unit) override {
    for (SensorAction *sensorAction : this->m_sensorActions) {
      sensorAction->discovery(id, device_type, unit);
    }
  }
};

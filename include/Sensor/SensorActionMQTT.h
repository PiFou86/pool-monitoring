#pragma once

#include "SensorAction.h"

class MQTTPubSubClient;
class SensorActionMQTT : public SensorAction {
 private:
  MQTTPubSubClient *m_mqttClient;

 public:
  SensorActionMQTT(MQTTPubSubClient *mqttClient);
  virtual void dataRead(const String &id, const String &device_type,
                        const String &unit, const float &value) override;
  virtual void discovery(const String &id, const String &device_type,
                         const String &unit) override;
  
  static String getStateTopic(const String &id, const String &device_type);
};

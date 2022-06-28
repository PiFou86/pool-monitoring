#pragma once

#include "SensorAction.h"

class MQTTPubSubClient;
class SensorActionMQTT : public SensorAction {
private:
    MQTTPubSubClient *m_mqttClient;

public:
    SensorActionMQTT(MQTTPubSubClient *mqttClient);
    virtual void dataRead(const String &id, const String &dataName, const String &unit, const float &value) override;
};

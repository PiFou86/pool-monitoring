#include "Sensor/SensorActionMQTT.h"

#include "Configuration/Configuration.h"
#include "Network/MQTTPubSubClient.h"

SensorActionMQTT::SensorActionMQTT(MQTTPubSubClient *mqttClient)
    : m_mqttClient(mqttClient) {
  ;
}

void SensorActionMQTT::dataRead(const String &id, const String &dataName,
                                const String &unit, const float &value) {
  String topic = Configuration.getMqttTopicPrefix() + "/" + id + "/" + dataName;
  String payload = String(value) + " " + unit;
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

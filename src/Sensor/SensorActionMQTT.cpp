#include "Sensor/SensorActionMQTT.h"

#include "Configuration/Configuration.h"
#include "Network/MQTTPubSubClient.h"

#include "Log/Logger.h"

SensorActionMQTT::SensorActionMQTT(MQTTPubSubClient *mqttClient)
    : m_mqttClient(mqttClient) {
  ;
}

void SensorActionMQTT::dataRead(const String &id, const String &dataName,
                                const String &unit, const float &value) {
  String topic = Configuration.getMqttTopicPrefix() + "/" + id + "/" + dataName;
  String payload = String(value) + " " + unit;
  Logger.infoln("Publishing to topic: " + topic + " with payload: " + payload);
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

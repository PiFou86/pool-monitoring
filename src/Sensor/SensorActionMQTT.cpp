#include "Sensor/SensorActionMQTT.h"

#include "Configuration/Configuration.h"
#include "Log/Logger.h"
#include "Network/MQTTPubSubClient.h"

SensorActionMQTT::SensorActionMQTT(MQTTPubSubClient *mqttClient)
    : m_mqttClient(mqttClient) {
  ;
}

void SensorActionMQTT::dataRead(const String &id, const String &dataName,
                                const String &unit, const float &value) {
  String topic =
      Configuration.getMqttTopicPrefix() + F("/") + id + F("/") + dataName;
  String payload = String(value) + F(" ") + unit;
  Logger.verboseln(String(F("Publishing to topic: ")) + topic +
                String(F(" with payload: ")) + payload);
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

void SensorActionMQTT::discovery(const String &id, const String &device_type,
                                 const String &name, const String &unit) {
  String topic = String(F("homeassistant/sensor/")) +
                 Configuration.getClientId() + id + name + String(F("/config"));
  String payload = String(F("{\"device_class\":\"")) + device_type +
                   String(F("\",\"name\":\"")) + name +
                   String(F("\",\"unit_of_measurement\":\"")) + unit +
                   String(F("\",\"state_topic\":\"")) + Configuration.getMqttStatus() +
                   String(F("\"}"));
  Logger.verboseln(String(F("Publishing to topic: ")) + topic +
                String(F(" with payload: ")) + payload);
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

#include "Sensor/SensorActionMQTT.h"

#include "Configuration/Configuration.h"
#include "Log/Logger.h"
#include "Network/MQTTPubSubClient.h"

SensorActionMQTT::SensorActionMQTT(MQTTPubSubClient *mqttClient)
    : m_mqttClient(mqttClient) {
  ;
}

void SensorActionMQTT::dataRead(const String &id, const String &device_type,
                                const String &unit, const float &value) {
  String topic = getStateTopic(id, device_type);
  String payload = String(value);
  Logger.verboseln(String(F("Publishing to topic: ")) + topic +
                   String(F(" with payload: ")) + payload);
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

void SensorActionMQTT::discovery(const String &id, const String &device_type,
                                 const String &unit) {
  String topic = String(F("homeassistant/sensor/")) +
                 Configuration.getClientId() + id + device_type +
                 String(F("/config"));
  String payload =
      String(F("{\"availability_topic\": \"")) + Configuration.getMqttStatus() +
      F("\"") + String(F(",\"device\": {")) + String(F("\"identifiers\":\"")) +
      Configuration.getMachineId() + F("\"") +
      String(F(",\"manufacturer\":\"PFL Technology\"")) +
      String(F(",\"model\":\"poolMonitoring PMSE01\"")) +
      String(F(",\"name\":\"Pool Monitoring System\"")) +
      String(F(",\"sw_version\":\"")) + ESP.getSketchMD5() + F("\"}") +
      String(F(",\"device_class\":\"")) + device_type +
      String(F("\",\"unique_id\":\"")) + Configuration.getClientId() + F("_") +
      id + device_type + String(F("\",\"name\":\"")) + id + F("_") + device_type +
      String(F("\",\"unit_of_measurement\":\"")) + unit +
      String(F("\",\"state_topic\":\"")) + getStateTopic(id, device_type) +
      String(F("\"}"));
  Logger.verboseln(String(F("Publishing to topic: ")) + topic +
                   String(F(" with payload: ")) + payload);
  this->m_mqttClient->publish(topic.c_str(), payload.c_str());
}

String SensorActionMQTT::getStateTopic(const String &id,
                                       const String &dataName) {
  return Configuration.getMqttTopicPrefix() + F("/") + id + F("/") + dataName;
}
#include "Network/MQTTPubSubClient.h"

#include <PubSubClient.h>

#include "Configuration/Configuration.h"
#include "Log/Logger.h"

MQTTPubSubClient::MQTTPubSubClient() {
  this->m_client = new PubSubClient();
  this->m_client->setServer(Configuration.getMqttServerIP(),
                            Configuration.getMqttServerPort());
}

void MQTTPubSubClient::reconnect() {
  if (!this->m_client->connected()) {
    Logger.infoln("MQTT: Reconnecting to MQTT server...");
    if (this->m_client->connect(Configuration.getClientId().c_str(),
                                Configuration.getMqttUser().c_str(),
                                Configuration.getMqttPassword().c_str())) {
      Logger.infoln("MQTT: Connected to MQTT server.");
    } else {
      Logger.errorln("MQTT: Failed to connect to MQTT server.");
    }
  }
}

void MQTTPubSubClient::publish(const char *topic, const char *payload) {
  this->reconnect();

  if (this->m_client->connected()) {
    if (this->m_client->publish(topic, payload)) {
      Logger.infoln("MQTT: Published message to topic '" + String(topic) +
                    "'.");
    } else {
      Logger.errorln("MQTT: Failed to publish message to topic '" +
                     String(topic) + "'.");
    }
  } else {
    Logger.errorln("MQTT: Not connected to MQTT server.");
  }
}
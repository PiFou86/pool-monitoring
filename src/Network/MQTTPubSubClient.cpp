#include "Network/MQTTPubSubClient.h"

#include <PubSubClient.h>

#include "Configuration/Configuration.h"
#include "Log/Logger.h"

#include "Network/WiFiConnection.h"

static WiFiClient espClient;

MQTTPubSubClient::MQTTPubSubClient() {
  this->m_client = new PubSubClient(espClient);
  this->m_client->setBufferSize(1024);
  Logger.infoln(F("MQTTPubSubClient initializing"));
  Logger.infoln(F("Setting MQTT broker..."));
  Logger.infoln(String(F("MQTT: IP: ")) +
                Configuration.getMqttServerIP().toString());
  Logger.infoln(String(F("MQTT: Port: ")) +
                String(Configuration.getMqttServerPort()));

  this->m_client->setServer(Configuration.getMqttServerIP(),
                            Configuration.getMqttServerPort());
}

void MQTTPubSubClient::reconnect() {
  if (WiFiConnection.reconnectIfNeeded() && !this->m_client->connected()) {
    Logger.infoln(F("MQTT: Reconnecting to MQTT server..."));
    Logger.infoln(String(F("MQTT: Client ID: ")) + Configuration.getClientId());
    Logger.verboseln(String(F("MQTT: User: ")) + Configuration.getMqttUser());
    Logger.verboseln(String(F("MQTT: Password: ")) +
                     Configuration.getMqttPassword());

    if (this->m_client->connect(Configuration.getClientId().c_str(),
                                Configuration.getMqttUser().c_str(),
                                Configuration.getMqttPassword().c_str(),
                                Configuration.getMqttWillTopic().c_str(), 0,
                                true, "offline")) {
      this->publish(Configuration.getMqttWillTopic().c_str(),
                              "online");
      Logger.infoln(String(F("MQTT: Connected to MQTT server.")));
    } else {
      Logger.errorln(String(F("MQTT: Failed to connect to MQTT server.")));
    }
  }
}

void MQTTPubSubClient::publish(const char *topic, const char *payload) {
  this->reconnect();

  if (this->m_client->connected()) {
    if (this->m_client->publish(topic, payload, true)) {
      Logger.verboseln(String(F("MQTT: message published to topic '")) +
                       String(topic) + String(F("'.")));
    } else {
      Logger.errorln(String(F("MQTT: Failed to publish message to topic '")) +
                     String(topic) + String(F("'.")));
    }
  } else {
    Logger.errorln(F("MQTT: Not connected to MQTT server."));
  }
}

void MQTTPubSubClient::tick() { this->m_client->loop(); }
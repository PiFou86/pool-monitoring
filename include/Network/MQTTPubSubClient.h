#pragma once

class PubSubClient;
class MQTTPubSubClient {
 private:
  PubSubClient *m_client;
  long m_lastReconnectAttempt;

 public:
  MQTTPubSubClient();
  void reconnect();
  void publish(const char *topic, const char *payload);
  void tick();
};

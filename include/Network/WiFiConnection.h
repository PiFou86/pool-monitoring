#pragma once

class WiFiManager;
class IPAddressParameter;
class IntParameter;
class FloatParameter;
class WiFiManagerParameter;
class WiFiConnectionImpl {
 private:
  WiFiManager *m_wifiManager;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WiFiManagerParameter *m_poolHeaterInAddress;
  WiFiManagerParameter *m_poolHeaterOutAddress;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  IPAddressParameter *m_mqtt_server_ip;
  IntParameter *m_mqtt_server_port;
  WiFiManagerParameter *m_mqtt_server_user;
  WiFiManagerParameter *m_mqtt_server_password;
  WiFiManagerParameter *m_mqtt_topic_prefix;

  void prepareWiFiManager();
  void prepareWiFiManagerParameters();
  void initParameterValues();
  bool tryToConnectToAP();

 public:
  WiFiConnectionImpl();
  void begin();
  bool startPortal();
};

extern WiFiConnectionImpl WiFiConnection;

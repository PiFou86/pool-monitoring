#include "Network/WiFiConnection.h"

#if ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif

#include <WiFiManager.h>

#include "Configuration.h"
#include "Configuration/Configuration.h"
#include "Log/Logger.h"

class IPAddressParameter : public WiFiManagerParameter {
 public:
  IPAddressParameter(const char *id, const char *placeholder, IPAddress address)
      : WiFiManagerParameter("") {
    init(id, placeholder, address.toString().c_str(), 16, "", WFM_LABEL_BEFORE);
  }

  bool getValue(IPAddress &ip) {
    return ip.fromString(WiFiManagerParameter::getValue());
  }
};

class IntParameter : public WiFiManagerParameter {
 public:
  IntParameter(const char *id, const char *placeholder, long value,
               const uint8_t length = 10)
      : WiFiManagerParameter("") {
    init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
  }

  long getValue() { return String(WiFiManagerParameter::getValue()).toInt(); }
};

class FloatParameter : public WiFiManagerParameter {
 public:
  FloatParameter(const char *id, const char *placeholder, float value,
                 const uint8_t length = 10)
      : WiFiManagerParameter("") {
    init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
  }

  float getValue() {
    return String(WiFiManagerParameter::getValue()).toFloat();
  }
};

WiFiConnectionImpl::WiFiConnectionImpl() {}

void WiFiConnectionImpl::begin() {
  this->prepareWiFiManager();
  this->tryToConnectToAP();
}

void WiFiConnectionImpl::prepareWiFiManager() {
  this->m_wifiManager = new WiFiManager();

  this->m_wifiManager->setAPCallback([](WiFiManager *p_wiFiManager) {
    Logger.infoln("Connexion au réseau WiFi échouée, on lance le portail !");
  });

  this->m_wifiManager->setSaveParamsCallback([this]() {
    Logger.infoln(
        "Sauvegarde de la configuration effectuée par l'utilisateur dans le "
        "portail...");

    Configuration.setPoolHeaterInAddress(
        this->m_poolHeaterInAddress->getValue());
    Configuration.setPoolHeaterOutAddress(
        this->m_poolHeaterOutAddress->getValue());

    IPAddress mqtt_server_ip;
    this->m_mqtt_server_ip->getValue(mqtt_server_ip);
    Configuration.setMqttServerIP(mqtt_server_ip);
    Configuration.setMqttServerPort(this->m_mqtt_server_port->getValue());
    Configuration.setMqttUser(this->m_mqtt_server_user->getValue());
    Configuration.setMqttPassword(this->m_mqtt_server_password->getValue());
    Configuration.setMqttTopicPrefix(this->m_mqtt_topic_prefix->getValue());

    Configuration.save();
    // Exemple d'actions...
    // Sauvegarde des données en JSON
    // Redémarrage : ESP.restart();
    // etc.
  });

  this->m_wifiManager->setConfigPortalTimeout(180);

  IPAddress adresseIPPortail(192, 168, 23, 1);
  IPAddress passerellePortail(192, 168, 23, 1);
  IPAddress masqueReseauPortail(255, 255, 255, 0);
  this->m_wifiManager->setAPStaticIPConfig(adresseIPPortail, passerellePortail,
                                           masqueReseauPortail);

  this->prepareWiFiManagerParameters();
}

void WiFiConnectionImpl::prepareWiFiManagerParameters() {
  this->m_mqtt_server_ip = new IPAddressParameter(
      "mqtt_server_ip", "MQTT Server IP", Configuration.getMqttServerIP());
  this->m_mqtt_server_port =
      new IntParameter("mqtt_server_port", "MQTT Server Port",
                       Configuration.getMqttServerPort(), 6);
  this->m_mqtt_server_user =
      new WiFiManagerParameter("mqtt_server_user", "MQTT Server User",
                               Configuration.getMqttUser().c_str(), 20);
  this->m_mqtt_server_password =
      new WiFiManagerParameter("mqtt_server_password", "MQTT Server Password",
                               Configuration.getMqttPassword().c_str(), 20);
  this->m_mqtt_topic_prefix =
      new WiFiManagerParameter("mqtt_topic_prefix", "MQTT Topic Prefix",
                               Configuration.getMqttTopicPrefix().c_str(), 50);

  this->m_wifiManager->setParamsPage(true);
  this->m_wifiManager->addParameter(this->m_poolHeaterInAddress);
  this->m_wifiManager->addParameter(this->m_poolHeaterOutAddress);

  this->m_wifiManager->addParameter(this->m_mqtt_server_ip);
  this->m_wifiManager->addParameter(this->m_mqtt_server_port);
  this->m_wifiManager->addParameter(this->m_mqtt_server_user);
  this->m_wifiManager->addParameter(this->m_mqtt_server_password);
  this->m_wifiManager->addParameter(this->m_mqtt_topic_prefix);
}

void WiFiConnectionImpl::initParameterValues() {
  this->m_poolHeaterInAddress->setValue(
      Configuration.getPoolHeaterInAddressAsString().c_str(), 32);
  this->m_poolHeaterOutAddress->setValue(
      Configuration.getPoolHeaterOutAddressAsString().c_str(), 32);

  this->m_mqtt_server_ip->setValue(Configuration.getMqttServerIP().toString().c_str(), 15);
  this->m_mqtt_server_port->setValue(String(Configuration.getMqttServerPort()).c_str(), 6);
  this->m_mqtt_server_user->setValue(Configuration.getMqttUser().c_str(), 20);
  this->m_mqtt_server_password->setValue(Configuration.getMqttPassword().c_str(), 20);
  this->m_mqtt_topic_prefix->setValue(Configuration.getMqttTopicPrefix().c_str(), 50);
}

bool WiFiConnectionImpl::tryToConnectToAP() {
  this->m_wifiManager->autoConnect(CONFIGURATION_PORTAL_SSID,
                                   CONFIGURATION_PORTAL_PASSWORD);

  return WiFi.status() == WL_CONNECTED;
}

bool WiFiConnectionImpl::startPortal() {
  this->m_wifiManager->startWebPortal();

  return WiFi.status() == WL_CONNECTED;
}

WiFiConnectionImpl WiFiConnection;
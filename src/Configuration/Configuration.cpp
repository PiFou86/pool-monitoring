#include "Configuration/Configuration.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

#if ESP8266
#include <LittleFS.h>
#elif ESP32
#include <SPIFFS.h>
#endif

#include "Log/Logger.h"

Configuration_Impl::Configuration_Impl()
    : m_isLoaded(false),
      m_isDirty(false),
      m_poolHeaterInAddress(nullptr),
      m_poolHeaterInAddressSize(0),
      m_poolHeaterOutAddress(nullptr),
      m_poolHeaterOutAddressSize(0) {
  ;
}

void Configuration_Impl::begin() {
#if ESP8266
  LittleFS.begin();
#elif ESP32
  SPIFFS.begin();
#endif

  this->load();
}

void Configuration_Impl::load() {
  StaticJsonDocument<256> doc;
#if ESP8266
  File file = LittleFS.open("/config.json", "r");
#elif ESP32
  File file = SPIFFS.open("/config.json", "r");
#endif
  if (!file) {
    Logger.errorln("Failed to open config.json");
    return;
  }

  DeserializationError error = deserializeJson(doc, file);

  if (error) {
    Logger.errorln("Failed to parse config.json");
    return;
  }

  this->setPoolHeaterInAddress(doc["poolHeaterInAddress"].as<String>());
  this->setPoolHeaterOutAddress(doc["poolHeaterOutAddress"].as<String>());

  IPAddress mqttServerIP;
  mqttServerIP.fromString(doc["mqttServerIP"].as<String>());
  this->setMqttServerIP(mqttServerIP);
  this->setMqttServerPort(doc["mqttServerPort"].as<uint16_t>());
  this->setMqttUser(doc["mqttUser"].as<String>());
  this->setMqttPassword(doc["mqttPassword"].as<String>());
  this->setMqttTopicPrefix(doc["mqttTopicPrefix"].as<String>());

  Logger.infoln("Configuration loaded");
  Logger.infoln("  poolHeaterInAddress: " +
                uintToString(this->m_poolHeaterInAddress,
                             this->m_poolHeaterInAddressSize));
  Logger.infoln("  poolHeaterOutAddress: " +
                uintToString(this->m_poolHeaterOutAddress,
                             this->m_poolHeaterOutAddressSize));
  Logger.infoln("");
  Logger.infoln("  mqttServerIP: " + this->m_mqttServerIP.toString());
  Logger.infoln("  mqttServerPort: " + this->m_mqttServerPort);
  Logger.infoln("  mqttUser: " + this->m_mqttUser);
  Logger.infoln("  mqttPassword: " + this->m_mqttPassword);
  Logger.infoln("  mqttTopicPrefix: " + this->m_mqttTopicPrefix);

  this->m_isLoaded = true;
  this->m_isDirty = false;
}

void Configuration_Impl::save() {
  StaticJsonDocument<256> doc;
  doc["poolHeaterInAddress"] = uintToString(
      this->m_poolHeaterInAddress, sizeof(this->m_poolHeaterInAddress));
  doc["poolHeaterOutAddress"] = uintToString(
      this->m_poolHeaterOutAddress, sizeof(this->m_poolHeaterOutAddress));

  doc["mqttServerIP"] = this->m_mqttServerIP.toString();
  doc["mqttServerPort"] = this->m_mqttServerPort;
  doc["mqttUser"] = this->m_mqttUser;
  doc["mqttPassword"] = this->m_mqttPassword;
  doc["mqttTopicPrefix"] = this->m_mqttTopicPrefix;

#if ESP8266
  File file = LittleFS.open("/config.json", "w");
#elif ESP32
  File file = SPIFFS.open("/config.json", "w");
#endif
  if (!file) {
    Logger.errorln("Failed to open config.json");
    return;
  }

  size_t error = serializeJson(doc, file);

  if (error) {
    Logger.errorln("Failed to write config.json");
    return;
  }

  this->m_isDirty = false;
}

uint8_t *Configuration_Impl::parseUintString(const String &str,
                                             size_t &addressSize) {
  std::vector<uint8_t> values;
  String strValue = str;

  int index = 0;
  while ((index = strValue.indexOf(':')) > 0) {
    int value = strValue.substring(0, index - 1).toInt();
    values.push_back(value);
    strValue = strValue.substring(index + 1);
  }

  uint8_t *uint = new uint8_t[values.size()];
  for (size_t i = 0; i < values.size(); i++) {
    uint[i] = values[i];
  }

  addressSize = values.size();

  return uint;
}

String Configuration_Impl::uintToString(uint8_t *uint, uint8_t length) {
  String str = "";

  str += String(uint[0]);
  for (uint8_t i = 1; i < length; i++) {
    str += ":" + String(uint[i]);
  }

  return str;
}

void Configuration_Impl::setPoolHeaterInAddress(
    const String &poolHeaterInAddress) {
  if (this->m_poolHeaterInAddress != nullptr) {
    delete[] this->m_poolHeaterInAddress;
    this->m_poolHeaterInAddress = nullptr;
    this->m_poolHeaterInAddressSize = 0;
  }

  this->m_poolHeaterInAddress =
      parseUintString(poolHeaterInAddress, this->m_poolHeaterInAddressSize);
  this->m_poolHeaterInAddressString = uintToString(
      this->m_poolHeaterInAddress, this->m_poolHeaterInAddressSize);
  this->dirty();
}

void Configuration_Impl::setPoolHeaterOutAddress(
    const String &poolHeaterOutAddress) {
  if (this->m_poolHeaterOutAddress != nullptr) {
    delete[] this->m_poolHeaterOutAddress;
    this->m_poolHeaterOutAddress = nullptr;
    this->m_poolHeaterOutAddressSize = 0;
  }

  this->m_poolHeaterOutAddress =
      parseUintString(poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  this->m_poolHeaterOutAddressString = uintToString(
      this->m_poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  this->dirty();
}

const String Configuration_Impl::clientId = String("pool_monitoring");

Configuration_Impl Configuration;

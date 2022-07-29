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

#include <Wire.h>

void testerAdresses();

Configuration_Impl::Configuration_Impl()
    : m_isLoaded(false),
      m_isDirty(false),
      m_poolHeaterInAddress(nullptr),
      m_poolHeaterInAddressSize(0),
      m_poolHeaterOutAddress(nullptr),
      m_poolHeaterOutAddressSize(0) {
  this->m_poolHeaterInAddress = new uint8_t[8];
  this->m_poolHeaterInAddressSize = 8;
  this->m_poolHeaterOutAddress = new uint8_t[8];
  this->m_poolHeaterOutAddressSize = 8;

  for (size_t i = 0; i < 8; i++) {
    this->m_poolHeaterInAddress[i] = 0;
    this->m_poolHeaterOutAddress[i] = 0;
  }

  this->setMqttServerIP(IPAddress(0, 0, 0, 0));
  this->setMqttServerPort(1883);
  this->setMqttUser("");
  this->setMqttPassword("");
  this->setMqttTopicPrefix("");
}

void Configuration_Impl::begin() {
  Logger.infoln("Configuration_Impl::begin()");
#if ESP8266
  LittleFS.begin();
#elif ESP32
  SPIFFS.begin(true);
#endif

  Wire.begin();
  testerAdresses();

  Logger.infoln("Loading configuration...");
  this->load();
}

void Configuration_Impl::load() {
  StaticJsonDocument<2048> doc;

  Logger.infoln("Loading configuration from file...");
#if ESP8266
  File file = LittleFS.open("/config.json", "r");
#elif ESP32
  File file = SPIFFS.open("/config.json", "r");
#endif
  if (!file) {
    Logger.errorln("Failed to open config.json");
    return;
  }

  Logger.infoln("Deserializing configuration...");
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Logger.errorln("Failed to parse config.json");
    return;
  }

  Logger.infoln("Setting configuration...");
  serializeJson(doc, Serial);

  Logger.infoln("Parsing DS18B20 addresses...");
  this->setPoolHeaterInAddress(doc["poolHeaterInAddress"].as<String>());
  this->setPoolHeaterOutAddress(doc["poolHeaterOutAddress"].as<String>());

  Logger.infoln("Parsing MQTT information...");
  IPAddress mqttServerIP;
  mqttServerIP.fromString(doc["mqttServerIP"].as<String>());
  this->setMqttServerIP(mqttServerIP);
  this->setMqttServerPort(doc["mqttServerPort"].as<uint16_t>());
  this->setMqttUser(doc["mqttUser"].as<String>());
  this->setMqttPassword(doc["mqttPassword"].as<String>());
  this->setMqttTopicPrefix(doc["mqttTopicPrefix"].as<String>());

  this->m_isLoaded = true;
  this->m_isDirty = false;

  Logger.infoln("Configuration loaded !");
  Logger.infoln("");

  this->printInfos();
}

void Configuration_Impl::save() {
  Logger.infoln("Saving configuration...");

  this->printInfos();

  StaticJsonDocument<4096> doc;
  doc["poolHeaterInAddress"] = this->m_poolHeaterInAddressString;
  doc["poolHeaterOutAddress"] = this->m_poolHeaterOutAddressString;

  doc["mqttServerIP"] = this->m_mqttServerIP.toString();
  doc["mqttServerPort"] = this->m_mqttServerPort;
  doc["mqttUser"] = this->m_mqttUser;
  doc["mqttPassword"] = this->m_mqttPassword;
  doc["mqttTopicPrefix"] = this->m_mqttTopicPrefix;

  Logger.infoln("Saving configuration to file...");
#if ESP8266
  File file = LittleFS.open("/config.json", "w");
#elif ESP32
  File file = SPIFFS.open("/config.json", "w");
#endif

  if (!file) {
    Logger.errorln("Failed to open config.json");
    return;
  }

  serializeJson(doc, Serial);
  size_t error = serializeJson(doc, file);
  file.close();

  if (error) {
    Logger.errorln("Failed to write config.json");
    return;
  }

  this->m_isDirty = false;
}

uint8_t *Configuration_Impl::parseAddressString(const String &str,
                                                size_t &addressSize) {
  Logger.infoln("Trying to parse " + str);
  std::vector<uint8_t> values;
  String strValue = str;

  int index = 0;
  while ((index = strValue.indexOf(':')) > 0) {
    String addressPart = strValue.substring(0, index);
    //Logger.infoln("Address part read : " + addressPart);
    int value = strtol(addressPart.c_str(), NULL, 16);
    values.push_back(value);
    strValue = strValue.substring(index + 1);
  }

  if (strValue.length() > 0) {
    //Logger.infoln("Address part read : " + strValue);
    int value = strtol(strValue.c_str(), NULL, 16);
    values.push_back(value);
  }

  uint8_t *uint = new uint8_t[values.size()];
  for (size_t i = 0; i < values.size(); i++) {
    uint[i] = (uint8_t)values[i];
  }

  addressSize = values.size();

  Logger.infoln("Parsing completed. Size : " + String(addressSize));

  return uint;
}

String Configuration_Impl::addressToString(uint8_t *uint, uint8_t length) {
  Logger.infoln("Trying to convert address in string (" + String(length) +
                " byte(s)...");
  String str = "";

  str += String(uint[0], 16);
  for (uint8_t i = 1; i < length; i++) {
    str += ":" + String(uint[i], 16);
  }

  return str;
}

void Configuration_Impl::setPoolHeaterInAddress(
    const String &poolHeaterInAddress) {
  Logger.infoln("Setting pool heater in address to " + poolHeaterInAddress);

  if (this->m_poolHeaterInAddress != nullptr) {
    delete[] this->m_poolHeaterInAddress;
    this->m_poolHeaterInAddress = nullptr;
    this->m_poolHeaterInAddressSize = 0;
  }

  this->m_poolHeaterInAddress =
      parseAddressString(poolHeaterInAddress, this->m_poolHeaterInAddressSize);
  this->m_poolHeaterInAddressString = addressToString(
      this->m_poolHeaterInAddress, this->m_poolHeaterInAddressSize);

  Logger.infoln("Pool heater in address set to " +
                this->m_poolHeaterInAddressString);
  this->dirty();
}

void Configuration_Impl::setPoolHeaterOutAddress(
    const String &poolHeaterOutAddress) {
  Logger.infoln("Setting pool heater out address to " + poolHeaterOutAddress);

  if (this->m_poolHeaterOutAddress != nullptr) {
    delete[] this->m_poolHeaterOutAddress;
    this->m_poolHeaterOutAddress = nullptr;
    this->m_poolHeaterOutAddressSize = 0;
  }

  this->m_poolHeaterOutAddress = parseAddressString(
      poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  this->m_poolHeaterOutAddressString = addressToString(
      this->m_poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  Logger.infoln("Pool heater out address set to " +
                this->m_poolHeaterOutAddressString);
  this->dirty();
}

void Configuration_Impl::printInfos() const {
  Logger.infoln("Configuration infos :");
  Logger.infoln("---------------------");
  Logger.infoln("  poolHeaterInAddress: " + this->m_poolHeaterInAddressString);
  Logger.infoln("  poolHeaterOutAddress: " +
                this->m_poolHeaterOutAddressString);
  Logger.infoln("  mqttServerIP: " + this->m_mqttServerIP.toString());
  Logger.infoln("  mqttServerPort: " + String(this->m_mqttServerPort));
  Logger.infoln("  mqttUser: " + this->m_mqttUser);
  Logger.infoln("  mqttPassword: " + this->m_mqttPassword);
  Logger.infoln("  mqttTopicPrefix: " + this->m_mqttTopicPrefix);
  Logger.infoln("---------------------");
  Logger.infoln("");
}

void testerAdresses() {
  byte error, address;  // variable for error and I2C address
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
}


const String Configuration_Impl::clientId = String("pool_monitoring");

Configuration_Impl Configuration;

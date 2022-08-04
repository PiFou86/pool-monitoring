#include "Configuration/Configuration.h"

#include <ArduinoJson.h>

#include <vector>

#include "Constantes.h"

#if ESP8266
#include <LittleFS.h>
#elif ESP32
#include <SPIFFS.h>
#endif

#include <DallasTemperature.h>
#include <MD5Builder.h>
#include <OneWire.h>
#include <Wire.h>

#include "Log/Logger.h"

Configuration_Impl::Configuration_Impl()
    : m_isLoaded(false),
      m_isDirty(false),
      m_poolHeaterInAddress(nullptr),
      m_poolHeaterInAddressSize(0),
      m_poolHeaterInAddressString(""),
      m_poolHeaterOutAddress(nullptr),
      m_poolHeaterOutAddressSize(0),
      m_poolHeaterOutAddressString(""),
      m_mqttServerIP(IPAddress(0, 0, 0, 0)),
      m_mqttServerPort(0),
      m_mqttUser(""),
      m_mqttPassword(""),
      m_mqttTopicPrefix(""),
      m_loggerLevel(LoggerLevel::INFO) {
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
  this->setLoggerLevel(LoggerLevel::INFO);

  this->m_isDirty = false;
}

void Configuration_Impl::begin() {
#if ESP8266
  LittleFS.begin();
#elif ESP32
  SPIFFS.begin(true);
#endif

  // Configuration_Impl::getI2CAddresses();

  Logger.infoln(F("Loading configuration..."));
  this->load();
}

void Configuration_Impl::load() {
  StaticJsonDocument<2048> doc;

  Logger.infoln(F("Loading configuration from file..."));
#if ESP8266
  File file = LittleFS.open("/config.json", "r");
#elif ESP32
  File file = SPIFFS.open("/config.json", "r");
#endif
  if (!file) {
    Logger.errorln(F("Failed to open config.json"));
    return;
  }

  Logger.verboseln(F("Deserializing configuration..."));
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Logger.errorln(F("Failed to parse config.json"));
    return;
  }

  Logger.verboseln(F("Setting configuration..."));
  serializeJson(doc, Serial);

  Logger.verboseln(F("Parsing DS18B20 addresses..."));
  this->setPoolHeaterInAddress(doc["poolHeaterInAddress"].as<String>());
  this->setPoolHeaterOutAddress(doc["poolHeaterOutAddress"].as<String>());

  Logger.verboseln(F("Parsing MQTT information..."));
  IPAddress mqttServerIP;
  mqttServerIP.fromString(doc["mqttServerIP"] | "8.8.8.8");
  this->setMqttServerIP(mqttServerIP);
  this->setMqttServerPort(doc["mqttServerPort"] | 1883);
  this->setMqttUser(doc["mqttUser"] | "mqttUser");
  this->setMqttPassword(doc["mqttPassword"] | "Passw0rd");
  this->setMqttTopicPrefix(doc["mqttTopicPrefix"] | "swimmingpool");
  this->setLoggerLevel(doc["loggerLevel"] | LoggerLevel::INFO);

  this->m_isLoaded = true;
  this->m_isDirty = false;

  Logger.infoln(F("Configuration loaded !"));
  Logger.infoln(F(""));

  this->printInfos();
}

void Configuration_Impl::save() {
  Logger.infoln(F("Saving configuration..."));

  this->printInfos();

  StaticJsonDocument<4096> doc;
  doc["poolHeaterInAddress"] = this->m_poolHeaterInAddressString;
  doc["poolHeaterOutAddress"] = this->m_poolHeaterOutAddressString;

  doc["mqttServerIP"] = this->m_mqttServerIP.toString();
  doc["mqttServerPort"] = this->m_mqttServerPort;
  doc["mqttUser"] = this->m_mqttUser;
  doc["mqttPassword"] = this->m_mqttPassword;
  doc["mqttTopicPrefix"] = this->m_mqttTopicPrefix;
  doc["loggerLevel"] = this->m_loggerLevel;

  Logger.verboseln(F("Saving configuration to file..."));
#if ESP8266
  File file = LittleFS.open("/config.json", "w");
#elif ESP32
  File file = SPIFFS.open("/config.json", "w");
#endif

  if (!file) {
    Logger.errorln(F("Failed to open config.json"));
    return;
  }

  serializeJson(doc, Serial);
  Serial.println();
  size_t byteCount = serializeJson(doc, file);
  file.close();

  if (byteCount == 0) {
    Logger.errorln(F("Failed to write config.json"));
    return;
  }

  this->m_isDirty = false;
}

uint8_t *Configuration_Impl::parseAddressString(const String &str,
                                                size_t &addressSize) {
  Logger.verboseln(String(F("Trying to parse ")) + str);
  std::vector<uint8_t> values;
  String strValue = str;

  int index = 0;
  while ((index = strValue.indexOf(':')) > 0) {
    String addressPart = strValue.substring(0, index);
    int value = strtol(addressPart.c_str(), NULL, 16);
    values.push_back(value);
    strValue = strValue.substring(index + 1);
  }

  if (strValue.length() > 0) {
    int value = strtol(strValue.c_str(), NULL, 16);
    values.push_back(value);
  }

  uint8_t *uint = new uint8_t[values.size()];
  for (size_t i = 0; i < values.size(); i++) {
    uint[i] = (uint8_t)values[i];
  }

  addressSize = values.size();

  Logger.verboseln(String(F("Parsing completed. Size : ")) +
                   String(addressSize));

  return uint;
}

String Configuration_Impl::addressToString(const uint8_t *uint,
                                           uint8_t length) {
  Logger.verboseln(String(F("Trying to convert address in string (")) +
                   String(length) + String(F(" byte(s)...")));
  String str = "";

  str += String(uint[0], HEX);
  for (uint8_t i = 1; i < length; i++) {
    str += ":" + String(uint[i], HEX);
  }

  return str;
}

void Configuration_Impl::setPoolHeaterInAddress(
    const String &poolHeaterInAddress) {
  Logger.verboseln(String(F("Setting pool heater in address to ")) +
                   poolHeaterInAddress);

  if (this->m_poolHeaterInAddress != nullptr) {
    delete[] this->m_poolHeaterInAddress;
    this->m_poolHeaterInAddress = nullptr;
    this->m_poolHeaterInAddressSize = 0;
    this->m_poolHeaterInAddressString = "";
  }

  this->m_poolHeaterInAddress =
      parseAddressString(poolHeaterInAddress, this->m_poolHeaterInAddressSize);
  this->m_poolHeaterInAddressString = addressToString(
      this->m_poolHeaterInAddress, this->m_poolHeaterInAddressSize);

  Logger.verboseln(String(F("Pool heater in address set to ")) +
                   this->m_poolHeaterInAddressString);
  this->dirty();
}

void Configuration_Impl::setPoolHeaterOutAddress(
    const String &poolHeaterOutAddress) {
  Logger.verboseln(String(F("Setting pool heater out address to ")) +
                   poolHeaterOutAddress);

  if (this->m_poolHeaterOutAddress != nullptr) {
    delete[] this->m_poolHeaterOutAddress;
    this->m_poolHeaterOutAddress = nullptr;
    this->m_poolHeaterOutAddressSize = 0;
    this->m_poolHeaterOutAddressString = "";
  }

  this->m_poolHeaterOutAddress = parseAddressString(
      poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  this->m_poolHeaterOutAddressString = addressToString(
      this->m_poolHeaterOutAddress, this->m_poolHeaterOutAddressSize);
  Logger.verboseln(String(F("Pool heater out address set to ")) +
                   this->m_poolHeaterOutAddressString);
  this->dirty();
}

void Configuration_Impl::printInfos() const {
  Logger.infoln(F("Configuration infos :"));
  Logger.infoln(F("---------------------"));
  Logger.infoln(String(F("  poolHeaterInAddress: ")) +
                this->m_poolHeaterInAddressString);
  Logger.infoln(String(F("  poolHeaterOutAddress: ")) +
                this->m_poolHeaterOutAddressString);
  Logger.infoln(String(F("  mqttServerIP: ")) +
                this->m_mqttServerIP.toString());
  Logger.infoln(String(F("  mqttServerPort: ")) +
                String(this->m_mqttServerPort));
  Logger.infoln(String(F("  mqttUser: ")) + this->m_mqttUser);
  Logger.infoln(String(F("  mqttPassword: ")) + this->m_mqttPassword);
  Logger.infoln(String(F("  mqttTopicPrefix: ")) + this->m_mqttTopicPrefix);
  Logger.infoln(String(F("  debug: ")) + String(this->m_loggerLevel));
  Logger.infoln(F("---------------------"));
  Logger.infoln(F(""));
}

std::vector<String> Configuration_Impl::getDS18B20SensorAddresses() {
  OneWire ow(ONE_WIRE_SENSOR_PIN);
  DallasTemperature DS18B20sensors(&ow);
  DS18B20sensors.begin();

  DeviceAddress Thermometer;

  size_t deviceCount = 0;
  Logger.infoln(F("Locating devices..."));
  Logger.info(F("Found "));
  deviceCount = DS18B20sensors.getDeviceCount();
  Logger.info(String(deviceCount), false);
  Logger.infoln(F(" devices."), false);
  Logger.infoln(F(""), false);

  Logger.infoln(F("Printing addresses..."));
  std::vector<String> addressesTemp;
  for (size_t i = 0; i < deviceCount; i++) {
    DS18B20sensors.getAddress(Thermometer, i);
    String address = Configuration.addressToString(Thermometer, 8);

    DS18B20sensors.requestTemperatures();

    float temperature = DS18B20sensors.getTempC(Thermometer);

    Logger.infoln(String(F(" - ")) + address + String(F(" : ")) +
                  String(temperature));
    addressesTemp.push_back(address + String(F(" : ")) + String(temperature));
  }

  return addressesTemp;
}

std::vector<uint16_t> Configuration_Impl::getI2CAddresses() {
  byte error, address;
  std::vector<uint16_t> i2cAddresses;

  Logger.infoln(F("Scanning I2C devices..."));

  Wire.begin();
  int nDevices = 0;
  for (address = 1; address < 127; address++) {
    Logger.verboseln(String(F("Sanning I2C devices at address 0x")) +
                     Logger.padLeft(String(address, HEX), 2, '0') + F(" !"));
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Logger.verboseln(String(F("I2C device found at address 0x")) +
                       Logger.padLeft(String(address, HEX), 2, '0') + F(" !"));
      ;

      i2cAddresses.push_back(address);
      nDevices++;
    } else if (error == 4) {
      Logger.infoln(String(F("Unknown error at address 0x")) +
                    Logger.padLeft(String(address, HEX), 2, '0') + F(" !"));
    }
  }
  if (nDevices == 0) {
    Logger.infoln(F("No I2C devices found."));
  } else {
    Logger.infoln(F("done."));
  }

  return i2cAddresses;
}

void Configuration_Impl::executeCommand(const String &command) {
  String cmd = command;
  cmd.trim();
  Logger.verboseln(String(F("Executing command : ")) + cmd);

  Logger.println(String(F("> ")) + cmd);

  String action = cmd.substring(0, cmd.indexOf(' '));
  String parameters = cmd.substring(cmd.indexOf(' ') + 1);

  if (action == "set") {
    String key = parameters.substring(0, parameters.indexOf(' '));
    String value = parameters.substring(parameters.indexOf(' ') + 1);

    if (key == "poolHeaterInAddress") {
      this->setPoolHeaterInAddress(value);
    } else if (key == "poolHeaterOutAddress") {
      this->setPoolHeaterOutAddress(value);
    } else if (key == "mqttServerPort") {
      this->setMqttServerPort(value.toInt());
    } else if (key == "mqttUser") {
      this->setMqttUser(value);
    } else if (key == "mqttPassword") {
      this->setMqttPassword(value);
    } else if (key == "mqttTopicPrefix") {
      this->setMqttTopicPrefix(value);
    } else if (key == "debug") {
      int debugLevel = value.toInt();
      if (debugLevel >= 0 && debugLevel <= 4) {
        Configuration.setLoggerLevel((LoggerLevel)debugLevel);
      } else {
        Logger.errorln(F("Invalid debug level"));
      }
    } else {
      Logger.errorln(String(F("Unknown key ")) + key);
    }
  } else if (action == "get") {
    String key = parameters;

    if (key == "poolHeaterInAddress") {
      Logger.infoln(String(F("poolHeaterInAddress: ")) +
                    this->m_poolHeaterInAddressString);
    } else if (key == "poolHeaterOutAddress") {
      Logger.infoln(String(F("poolHeaterOutAddress: ")) +
                    this->m_poolHeaterOutAddressString);
    } else if (key == "mqttServerIP") {
      Logger.infoln(String(F("mqttServerIP: ")) +
                    this->m_mqttServerIP.toString());
    } else if (key == "mqttServerPort") {
      Logger.infoln(String(F("mqttServerPort: ")) +
                    String(this->m_mqttServerPort));
    } else if (key == "mqttUser") {
      Logger.infoln(String(F("mqttUser: ")) + this->m_mqttUser);
    } else if (key == "mqttPassword") {
      Logger.infoln(String(F("mqttPassword: ")) + this->m_mqttPassword);
    } else if (key == "mqttTopicPrefix") {
      Logger.infoln(String(F("mqttTopicPrefix: ")) + this->m_mqttTopicPrefix);
    } else if (key == "debug") {
      Logger.infoln(String(F("debug: ")) + Configuration.getLoggerLevel());
    } else {
      Logger.errorln(String(F("Unknown key ")) + key);
    }
  } else if (action == "save") {
    Configuration.save();
  } else if (action == "infos") {
    Configuration.printInfos();
  } else if (action == "reboot") {
    Logger.infoln(F("Rebooting..."));
    ESP.restart();
  } else if (action == "scan") {
    String deviceType = parameters;

    if (deviceType == "i2c") {
      std::vector<uint16_t> i2cAddresses = Configuration.getI2CAddresses();
      Logger.println(F("I2C addresses:"));
      for (size_t i = 0; i < i2cAddresses.size(); i++) {
        Logger.println(String(F("  - 0x")) +
                       Logger.padLeft(String(i2cAddresses[i], HEX), 2, '0'));
      }
      Logger.println(String(F("")));
    } else if (deviceType == "ds18b20") {
      std::vector<String> addresses = Configuration.getDS18B20SensorAddresses();
      Logger.println(F("DS18B20 addresses:"));
      for (size_t i = 0; i < addresses.size(); i++) {
        Logger.println(String("  - ") + addresses[i]);
      }
      Logger.println(String(F("")));
    } else if (deviceType == "id") {
      Logger.println(String(F("ID: ")) + Configuration.getMachineId());
    } else {
      Logger.errorln(String(F("Unknown device type ")) + deviceType);
    }
  } else if (action == "help") {
    Logger.println(F("Available commands:"));
    Logger.println(F(" Configuration:"));
    Logger.println(F("  infos               | Print configuration infos"));
    Logger.println(
        F("  get <key>           | Get the value of a configuration key"));
    Logger.println(
        F("  set <key> <value>   | Set configuration value (needs reboot)"));
    Logger.println(F("  save                | Save configuration"));
    Logger.println(F("  reboot              | Reboot the device"));
    Logger.println(F(""));
    Logger.println(F(" Tools:"));
    Logger.println(
        F("  scan <deviceType>   | Available device types : i2c, ds18b20"));
    Logger.println(F("  id                  | Print the device ID"));
    Logger.println(F(""));
    Logger.println(F("  help"));
  } else {
    Logger.errorln(String(F("Unknown action ")) + action);
  }
}

String Configuration_Impl::getMachineId() {
  // MD5Builder md5;
  // md5.begin();
  // md5.add(String(ESP.getEfuseMac() & 0xFFFFFFFF));
  // md5.calculate();
  // return md5.toString();

  uint32_t macPart = ESP.getEfuseMac() & 0xFFFFFFFF;
  return String(macPart, HEX);
}

const String Configuration_Impl::clientId = String("pool_monitoring");

Configuration_Impl Configuration;

#pragma once

#include <Arduino.h>
#include <vector>

#if ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif

#include "Log/Logger.h"

class Configuration_Impl {
private:
    bool m_isLoaded;
    bool m_isDirty;
    
    uint8_t * m_poolHeaterInAddress;
    size_t m_poolHeaterInAddressSize;
    String m_poolHeaterInAddressString;
    uint8_t * m_poolHeaterOutAddress;
    size_t m_poolHeaterOutAddressSize;
    String m_poolHeaterOutAddressString;

    IPAddress m_mqttServerIP;
    uint16_t m_mqttServerPort;
    String m_mqttUser;
    String m_mqttPassword;
    String m_mqttTopicPrefix;

    LoggerLevel m_loggerLevel;

    static const String clientId;

public:
    Configuration_Impl();

    void begin();

    void load();
    void save();

    inline bool isLoaded() const { return this->m_isLoaded; };
    inline bool isDirty() const { return this->m_isDirty; };
    inline void dirty() { this->m_isDirty = true; };
    inline void clean() { this->m_isDirty = false; };

    ///////////////////////////////////////////////////////////////////////////
    inline uint8_t *getPoolHeaterInAddress() const { return this->m_poolHeaterInAddress; };
    inline const String &getPoolHeaterInAddressAsString() const { return this->m_poolHeaterInAddressString; };
    void setPoolHeaterInAddress(const String &poolHeaterInAddress);

    inline uint8_t *getPoolHeaterOutAddress() const { return this->m_poolHeaterOutAddress; };
    inline const String &getPoolHeaterOutAddressAsString() const { return this->m_poolHeaterOutAddressString; };
    void setPoolHeaterOutAddress(const String &poolHeaterOutAddress);


    ///////////////////////////////////////////////////////////////////////////
    inline IPAddress getMqttServerIP() const { return this->m_mqttServerIP; };
    inline void setMqttServerIP(const IPAddress &mqttServerIP) { this->m_mqttServerIP = mqttServerIP; this->dirty(); };

    inline uint16_t getMqttServerPort() const { return this->m_mqttServerPort; };
    inline void setMqttServerPort(const uint16_t &mqttServerPort) { this->m_mqttServerPort = mqttServerPort; this->dirty(); };

    inline const String & getMqttUser() const { return this->m_mqttUser; };
    inline void setMqttUser(const String &mqttUser) { this->m_mqttUser = mqttUser; this->dirty(); };

    inline const String & getMqttPassword() const { return this->m_mqttPassword; };
    inline void setMqttPassword(const String &mqttPassword) { this->m_mqttPassword = mqttPassword; this->dirty(); };

    const String & getMqttTopicPrefix() const { return this->m_mqttTopicPrefix; };
    inline void setMqttTopicPrefix(const String &mqttTopicPrefix) { this->m_mqttTopicPrefix = mqttTopicPrefix; this->dirty(); };

    inline unsigned long getReadInterval() const { return 15000; };
    inline const String &getClientId() const { return clientId; };

    void printInfos() const;

    ///////////////////////////////////////////////////////////////////////////
    inline String getMqttStatus() const { return String(F("homeassistant/sensor/")) + this->m_mqttTopicPrefix + String(F("/status")); };
    inline String getMqttWillTopic() const { return this->getMqttStatus(); };


    ///////////////////////////////////////////////////////////////////////////
    inline LoggerLevel getLoggerLevel() const { return this->m_loggerLevel; };
    inline void setLoggerLevel(const LoggerLevel &loggerLevel) { this->m_loggerLevel = loggerLevel; this->dirty(); };


    ///////////////////////////////////////////////////////////////////////////
    void executeCommand(const String &command);

    static uint8_t *parseAddressString(const String &str, size_t &addressSize);
    static String addressToString(const uint8_t *uint, uint8_t length);
    static std::vector<String> getDS18B20SensorAddresses();
    static std::vector<u16_t> getI2CAddresses();
};

extern Configuration_Impl Configuration;
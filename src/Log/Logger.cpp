#include "Configuration.h"
#include "Log/Logger.h"

void LoggerImpl::begin() {
    Serial.begin(SERIAL_BAUD);
}

void LoggerImpl::error(const String &value) {
    this->print("ERROR: " + value);
}
void LoggerImpl::errorln(const String &value) {
    this->println("ERROR: " + value);
}
void LoggerImpl::error(const char *value) {
    this->print("ERROR: " + String(value));
}
void LoggerImpl::errorln(const char *value) {
    this->println("ERROR: " + String(value));
}

void LoggerImpl::warning(const String &value) {
    this->print("WARN: " + value);
}
void LoggerImpl::warningln(const String &value) {
    this->println("WARN: " + value);
}
void LoggerImpl::warning(const char *value) {
    this->print("WARN: " + String(value));
}
void LoggerImpl::warningln(const char *value) {
    this->println("WARN: " + String(value));
}

void LoggerImpl::info(const String &value) {
    this->print("INFO: " + value);
}
void LoggerImpl::infoln(const String &value) {
    this->println("INFO: " + value);
}
void LoggerImpl::info(const char *value) {
    this->print("INFO: " + String(value));
}
void LoggerImpl::infoln(const char *value) {
    this->println("INFO: " + String(value));
}

void LoggerImpl::print(const String &value) {
    Serial.print(value);
}
void LoggerImpl::println(const String &value) {
    Serial.println(value);
}

LoggerImpl Logger;

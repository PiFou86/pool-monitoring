#include "Log/Logger.h"

#include "Configuration.h"

void LoggerImpl::begin() { Serial.begin(SERIAL_BAUD); }

void LoggerImpl::error(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->print("ERROR: " + value);
  } else {
    this->print(value);
  }
}
void LoggerImpl::errorln(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->println("ERROR: " + value);
  } else {
    this->println(value);
  }
}
void LoggerImpl::error(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->print("ERROR: " + String(value));
  } else {
    this->print(String(value));
  }
}
void LoggerImpl::errorln(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->println("ERROR: " + String(value));
  } else {
    this->println(String(value));
  }
}

void LoggerImpl::warning(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->print("WARN: " + value);
  } else {
    this->print(value);
  }
}
void LoggerImpl::warningln(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->println("WARN: " + value);
  } else {
    this->println(value);
  }
}
void LoggerImpl::warning(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->print("WARN: " + String(value));
  } else {
    this->print(String(value));
  }
}
void LoggerImpl::warningln(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->println("WARN: " + String(value));
  } else {
    this->println(String(value));
  }
}

void LoggerImpl::info(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->print("INFO: " + value);
  } else {
    this->print(value);
  }
}
void LoggerImpl::infoln(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->println("INFO: " + value);
  } else {
    this->println(value);
  }
}
void LoggerImpl::info(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->print("INFO: " + String(value));
  } else {
    this->print(String(value));
  }
}
void LoggerImpl::infoln(const char *value, bool writePrefix) {
  if (writePrefix) {
    this->println("INFO: " + String(value));
  } else {
    this->println(String(value));
  }
}

void LoggerImpl::print(const String &value) { Serial.print(value); }
void LoggerImpl::println(const String &value) { Serial.println(value); }

LoggerImpl Logger;

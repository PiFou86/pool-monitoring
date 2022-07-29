#pragma once
#include <Arduino.h>

class LoggerImpl {
public:
    void begin();

    void error(const String &value, bool writePrefix = true);
    void errorln(const String &value, bool writePrefix = true);
    void error(const char *value, bool writePrefix = true);
    void errorln(const char *value, bool writePrefix = true);

    void warning(const String &value, bool writePrefix = true);
    void warningln(const String &value, bool writePrefix = true);
    void warning(const char *value, bool writePrefix = true);
    void warningln(const char *value, bool writePrefix = true);

    void info(const String &value, bool writePrefix = true);
    void infoln(const String &value, bool writePrefix = true);
    void info(const char *value, bool writePrefix = true);
    void infoln(const char *value, bool writePrefix = true);
private:
    void print(const String &value);
    void println(const String &value);
};

extern LoggerImpl Logger;

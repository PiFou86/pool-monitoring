#pragma once
#include <Arduino.h>

class LoggerImpl {
public:
    void begin();

    void error(const String &value);
    void errorln(const String &value);
    void error(const char *value);
    void errorln(const char *value);

    void warning(const String &value);
    void warningln(const String &value);
    void warning(const char *value);
    void warningln(const char *value);

    void info(const String &value);
    void infoln(const String &value);
    void info(const char *value);
    void infoln(const char *value);
private:
    void print(const String &value);
    void println(const String &value);
};

extern LoggerImpl Logger;

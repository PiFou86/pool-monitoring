#include "Log/Logger.h"

#include "Constantes.h"

#include "Configuration/Configuration.h"

void LoggerImpl::begin(Stream *stream) { this->m_stream = stream; }

void LoggerImpl::error(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::ERROR) {
    if (writePrefix) {
      this->print(String(F("ERROR: ")) + value, writePrefix);
    } else {
      this->print(value, writePrefix);
    }
  }
}
void LoggerImpl::errorln(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::ERROR) {
    if (writePrefix) {
      this->println(String(F("ERROR: ")) + value, writePrefix);
    } else {
      this->println(value, writePrefix);
    }
  }
}
void LoggerImpl::error(const char *value, bool writePrefix) {
  this->error(String(value), writePrefix);
}
void LoggerImpl::errorln(const char *value, bool writePrefix) {
  this->errorln(String(value), writePrefix);
}

void LoggerImpl::warning(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::WARNING) {
    if (writePrefix) {
      this->print(String(F("WARNING: ")) + value, writePrefix);
    } else {
      this->print(value, writePrefix);
    }
  }
}
void LoggerImpl::warningln(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::WARNING) {
    if (writePrefix) {
      this->println(String(F("WARNING: ")) + value, writePrefix);
    } else {
      this->println(value, writePrefix);
    }
  }
}

void LoggerImpl::warning(const char *value, bool writePrefix) {
  this->warning(String(value), writePrefix);
}

void LoggerImpl::warningln(const char *value, bool writePrefix) {
  this->warningln(String(value), writePrefix);
}

void LoggerImpl::info(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::INFO) {
    if (writePrefix) {
      this->print(String(F("INFO: ")) + value, writePrefix);
    } else {
      this->print(value, writePrefix);
    }
  }
}
void LoggerImpl::infoln(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::INFO) {
    if (writePrefix) {
      this->println(String(F("INFO: ")) + value, writePrefix);
    } else {
      this->println(value, writePrefix);
    }
  }
}

void LoggerImpl::info(const char *value, bool writePrefix) {
  this->info(String(value), writePrefix);
}

void LoggerImpl::infoln(const char *value, bool writePrefix) {
  this->infoln(String(value), writePrefix);
}

void LoggerImpl::verbose(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::VERBOSE) {
    if (writePrefix) {
      this->print(String(F("VERBOSE: ")) + value, writePrefix);
    } else {
      this->print(value, writePrefix);
    }
  }
}
void LoggerImpl::verboseln(const String &value, bool writePrefix) {
  if (Configuration.getLoggerLevel() >= LoggerLevel::VERBOSE) {
    if (writePrefix) {
      this->println(String(F("VERBOSE: ")) + value, writePrefix);
    } else {
      this->println(value, writePrefix);
    }
  }
}
void LoggerImpl::verbose(const char *value, bool writePrefix) {
  this->verbose(String(value), writePrefix);
}

void LoggerImpl::verboseln(const char *value, bool writePrefix) {
  this->verboseln(String(value), writePrefix);
}

void LoggerImpl::print(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->m_stream->print(millisToTime(millis()) + String(F(" - ")));
  }
  this->m_stream->print(value);
}

void LoggerImpl::println(const String &value, bool writePrefix) {
  if (writePrefix) {
    this->m_stream->print(millisToTime(millis()) + String(F(" - ")));
  }
  this->m_stream->println(value);
}

String LoggerImpl::millisToTime(unsigned long millis) {
  unsigned long seconds = millis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  // unsigned long years = days / 365;
  // unsigned long months = years / 12;
  // unsigned long yearsRemainder = years % 12;
  // unsigned long monthsRemainder = months % 12;
  // unsigned long daysRemainder = days % 365;
  unsigned long hoursRemainder = hours % 24;
  unsigned long minutesRemainder = minutes % 60;
  unsigned long secondsRemainder = seconds % 60;
  unsigned long millisRemainder = millis % 1000;

  // String(years) + String(F("y ")) + String(monthsRemainder) + String(F("m "))
  // + String(daysRemainder) + String(F("d ")) +

  String result = padLeft(String(hoursRemainder), '0', 2) + String(F(":")) +
                  padLeft(String(minutesRemainder), '0', 2) + String(F(":")) +
                  padLeft(String(secondsRemainder), '0', 2) + String(F(".")) +
                  padLeft(String(millisRemainder), '0', 3);
  return result;
}

String LoggerImpl::padLeft(const String &value, char padChar, int length) {
  String result = value;
  while (result.length() < length) {
    result = padChar + result;
  }
  return result;
}

LoggerImpl Logger;

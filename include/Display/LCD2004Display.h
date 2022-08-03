#pragma once
#include <Arduino.h>

#include "Readiness.h"

class LiquidCrystal_I2C;
class LCD2004Display : public Readiness {
 private:
  LiquidCrystal_I2C *m_lcd;
  float m_waterPressure;

  float m_waterInTemperature;
  float m_waterOutTemperature;

  float m_outdoorTemperature;
  float m_outdoorHumidity;
  float m_outdoorPressure;

  unsigned long m_lastUpdate;
  size_t m_currentLine;
  unsigned long m_startedBacklightDate;
  bool m_backlightIsOn;

  bool m_isReady;

  String getLine(const size_t &lineNumber) const;
  void setBacklight(bool isOn);

  inline void ready() { this->m_isReady = true; }
  inline void notReady() { this->m_isReady = false; }

  inline bool isDirty() const { return this->m_isDirty; }
  inline void dirty() { this->m_isDirty = true; }

  String m_messages[4];
  uint8_t m_messageIndex;
  bool m_isDirty;

 public:
  LCD2004Display();
  void tick();
  void setValue(const String &name, const float &value);
  void backlight();

  void displayMessage(const String &message, const bool withBacklight = true);

  void clearMessages();

  inline bool isReady() const { return this->m_isReady; }
};

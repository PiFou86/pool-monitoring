#pragma once
#include <Arduino.h>

class LiquidCrystal_I2C;
class LCD1602Display {
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

  String getLine(const size_t &lineNumber) const;
  void setBacklight(bool isOn);

 public:
  LCD1602Display();
  void tick();
  void setValue(const String &name, const float &value);
  void backlight();
};

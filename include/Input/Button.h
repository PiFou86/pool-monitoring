#pragma once

#include <Arduino.h>

class ButtonAction;
class Button {
    private:
      uint8_t m_pin;
      ButtonAction *m_pressAction;
      ButtonAction *m_longPressAction;
      unsigned long m_lastChangeDate;
      int m_lastState;
      int m_lastStableState;

 public:
  Button(uint8_t pin, ButtonAction *pressAction, ButtonAction *longPressAction);
  void tick();
};
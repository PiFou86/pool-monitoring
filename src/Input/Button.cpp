#include "Input/Button.h"

#include "Constantes.h"
#include "Input/ButtonAction.h"

#include "Log/Logger.h"

Button::Button(uint8_t pin, ButtonAction *pressAction, ButtonAction *longPressAction)
    : m_pin(pin),
      m_pressAction(pressAction),
      m_longPressAction(longPressAction),
      m_lastChangeDate(0),
      m_lastState(HIGH),
      m_lastStableState(HIGH) {
  pinMode(this->m_pin, INPUT);
}

void Button::tick() {
  int buttonState = digitalRead(this->m_pin);
  long currentDate = millis();

  if (buttonState != m_lastState) {
    m_lastChangeDate = currentDate;
    m_lastState = buttonState;
  }

  if (currentDate - this->m_lastChangeDate > BUTTON_DEBOUNCE_DELAY) {
    // Button released
    if (this->m_lastStableState == LOW && buttonState == HIGH) {
      Logger.verboseln(F("Button released"));
      this->m_pressAction->execute();
    } else {
      // Long press
      if (this->m_lastStableState == LOW && buttonState == LOW) {
        if (currentDate - this->m_lastChangeDate > BUTTON_LONG_PRESS_DELAY) {
          Logger.verboseln(F("Button long pressed"));
          this->m_lastChangeDate = currentDate;
          this->m_longPressAction->execute();
        }
      }
    }

    this->m_lastStableState = buttonState;
  }
}

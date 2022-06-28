#include "Input/Button.h"

#include "Configuration.h"
#include "Input/ButtonAction.h"

Button::Button(uint8_t pin, ButtonAction *pressAction, ButtonAction *longPressAction)
    : m_pin(pin),
      m_pressAction(pressAction),
      m_longPressAction(longPressAction),
      m_lastChangeDate(0),
      m_lastState(HIGH),
      m_lastStableState(HIGH) {
  pinMode(this->m_pin, INPUT_PULLUP);
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
      this->m_pressAction->execute();
    } else {
      // Long press
      if (this->m_lastStableState == LOW && buttonState == LOW) {
        if (currentDate - this->m_lastChangeDate > BUTTON_LONG_PRESS_DELAY) {
          this->m_longPressAction->execute();
        }
      }
    }

    this->m_lastStableState = buttonState;
  }
}

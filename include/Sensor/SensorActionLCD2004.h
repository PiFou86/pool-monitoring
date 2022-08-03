#include "Display/LCD2004Display.h"
#include "SensorAction.h"

class SensorActionLCD2004 : public SensorAction {
 private:
  LCD2004Display *m_lcdDisplay;

 public:
  inline SensorActionLCD2004(LCD2004Display *lcdDisplay) { this->m_lcdDisplay = lcdDisplay; };
  inline virtual void dataRead(const String &id, const String &dataName,
                               const String &unit,
                               const float &value) override {
    this->m_lcdDisplay->setValue(id + String(F("_")) + dataName, value);
  }
  inline virtual void discovery(const String &id, const String &device_type,
                         const String &name, const String &unit) override {
    ;
  }
};

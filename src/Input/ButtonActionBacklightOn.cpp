#include "Input/ButtonActionBacklightOn.h"

#include "Display/LCD2004Display.h"

ButtonActionBacklightOn::ButtonActionBacklightOn(LCD2004Display *lcd)
    : m_lcd(lcd) {}

void ButtonActionBacklightOn::execute() {
    this->m_lcd->backlight();
}

#include "Input/ButtonActionBacklightOn.h"

#include "Display/LCD1602Display.h"

ButtonActionBacklightOn::ButtonActionBacklightOn(LCD1602Display *lcd)
    : m_lcd(lcd) {}

void ButtonActionBacklightOn::execute() {
    this->m_lcd->backlight();
}

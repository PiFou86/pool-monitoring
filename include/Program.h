#pragma once
#include <vector>

class Sensor;
class OneWire;
class Button;
class LCD1602Display;
class Program {
public:
    Program();
    void loop();

private:
    std::vector<Sensor *> m_sensors;
    OneWire *m_oneWire;
    unsigned long m_lastRead;
    Button *m_diagnosticButton;
    LCD1602Display *m_lcd;
};

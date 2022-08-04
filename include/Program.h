#pragma once
#include <vector>
#include <Arduino.h>

class Sensor;
class OneWire;
class Button;
class LCD2004Display;
class MQTTPubSubClient;
class Program {
public:
    Program();
    void loop();

private:
    std::vector<Sensor *> m_sensors;
    OneWire *m_oneWire;
    unsigned long m_lastRead;
    Button *m_diagnosticButton;
    LCD2004Display *m_lcd;
    String m_command;
    MQTTPubSubClient *m_mqttClient;
};

#include "Program.h"

#include <OneWire.h>

#include "Configuration.h"
#include "Configuration/Configuration.h"

#include "Display/LCD1602Display.h"

#include "Input/Button.h"
#include "Input/ButtonActionBacklightOn.h"
#include "Input/ButtonActionNull.h"
#include "Input/ButtonActionConfiguration.h"

#include "Log/Logger.h"

#include "Network/WiFiConnection.h"
#include "Network/MQTTPubSubClient.h"

#include "Sensor/AnalogPressureSensor.h"
#include "Sensor/BME280Sensor.h"
#include "Sensor/DS18B20Sensor.h"
#include "Sensor/SensorActionNull.h"
#include "Sensor/SensorActionMQTT.h"

Program::Program() {
  Logger.begin();
  Logger.infoln("Program started");
  Logger.infoln("Initializing configuration");
  Configuration.begin();
  Logger.infoln("Configuration initialized");

  Logger.infoln("Initializing display");
  this->m_lcd = new LCD1602Display();
  Logger.infoln("Display initialized");

  Logger.infoln("Initializing network");
  WiFiConnection.begin();
  Logger.infoln("Network initialized");

  Logger.infoln("Initializing sensors");
  this->m_oneWire = new OneWire(2);
  MQTTPubSubClient *mqttPubSubClient = new MQTTPubSubClient();
  SensorActionMQTT *sensorActionMQTT = new SensorActionMQTT(mqttPubSubClient);
  this->m_sensors.push_back(
      new BME280Sensor("pool_outdoor", sensorActionMQTT));
  this->m_sensors.push_back(new DS18B20Sensor(
      "pool_heater_in", sensorActionMQTT, this->m_oneWire,
      Configuration.getPoolHeaterInAddress()));
  this->m_sensors.push_back(new DS18B20Sensor(
      "pool_heater_out", sensorActionMQTT, this->m_oneWire,
      Configuration.getPoolHeaterOutAddress()));
  this->m_sensors.push_back(new AnalogPressureSensor(
      "pool_pressure", sensorActionMQTT, A0, 0, 30));
  Logger.infoln("Sensors initialized");

  Logger.infoln("Initializing inputs");
  this->m_diagnosticButton = new Button(
      DIAGNOSTIC_PUSH_BUTTON_PIN, new ButtonActionBacklightOn(this->m_lcd),
      new ButtonActionConfiguration());
  Logger.infoln("Inputs initialized");

  this->m_lastRead = 0;
}

void Program::loop() {
  unsigned long currentDate = millis();

  if (currentDate - this->m_lastRead > Configuration.getReadInterval()) {
    Logger.infoln("Reading sensors");
    this->m_lastRead = currentDate;

    for (Sensor *sensor : this->m_sensors) {
      sensor->readAndInform();
    }
  }
}
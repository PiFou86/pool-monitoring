#include "Program.h"

#include <OneWire.h>

#include "Configuration.h"
#include "Configuration/Configuration.h"
#include "Input/Button.h"
#include "Input/ButtonActionNull.h"
#include "Log/Logger.h"
#include "Network/WiFiConnection.h"
#include "Sensor/AnalogPressureSensor.h"
#include "Sensor/BME280Sensor.h"
#include "Sensor/DS18B20Sensor.h"
#include "Sensor/SensorActionNull.h"

Program::Program() {
  Logger.begin();
  Logger.infoln("Program started");
  Logger.infoln("Initializing configuration");
  Configuration.begin();
  Logger.infoln("Configuration initialized");
  Logger.infoln("Initializing network");
  WiFiConnection.begin();
  Logger.infoln("Network initialized");

  Logger.infoln("Initializing sensors");
  this->m_oneWire = new OneWire(2);
  this->m_sensors.push_back(new BME280Sensor("pool_outdoor", new SensorActionNull()));
  this->m_sensors.push_back(new DS18B20Sensor("pool_heater_in", new SensorActionNull(), this->m_oneWire, Configuration.getPoolHeaterInAddress()));
  this->m_sensors.push_back(new DS18B20Sensor("pool_heater_out", new SensorActionNull(), this->m_oneWire, Configuration.getPoolHeaterOutAddress()));
  this->m_sensors.push_back(new AnalogPressureSensor("pool_pressure", new SensorActionNull(), A0, 0, 30));
  Logger.infoln("Sensors initialized");

  Logger.infoln("Initializing inputs");
  this->m_diagnosticButton = new Button(DIAGNOSTIC_PUSH_BUTTON_PIN, new ButtonActionNull(), new ButtonActionNull());
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
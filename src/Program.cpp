#include "Program.h"

#include <OneWire.h>

#include "Configuration.h"
#include "Configuration/Configuration.h"

#include "Display/LCD2004Display.h"

#include "Input/Button.h"
#include "Input/ButtonActionBacklightOn.h"
#include "Input/ButtonActionNull.h"
#include "Input/ButtonActionConfiguration.h"

#include "Log/Logger.h"
#include "Log/GeneralInformation.h"
#include "Log/LCD2004MessageDisplay.h"

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


  OneWire ow(ONE_WIRE_SENSOR_PIN);
  DallasTemperature DS18B20sensors(&ow);

  DeviceAddress Thermometer;

  int deviceCount = 0;
  Logger.infoln("Locating devices...");
  Logger.info("Found ");
  deviceCount = DS18B20sensors.getDeviceCount();
  Logger.info(String(deviceCount), false);
  Logger.infoln(" devices.", false);
  Logger.infoln("", false);

  Logger.infoln("Printing addresses...");
  std::vector<String> addresses;
  for (int i = 0; i < deviceCount; i++) {
    DS18B20sensors.getAddress(Thermometer, i);
    String address = Configuration.addressToString(Thermometer, 8);
    Logger.infoln(" - " + address);
    addresses.push_back(address);
  }

  Configuration.begin();
  Logger.infoln("Configuration initialized");

  Logger.infoln("Initializing display");
  this->m_lcd = new LCD2004Display();
  GeneralInformation.setMessageDisplay(new LCD2004MessageDisplay(this->m_lcd));
  GeneralInformation.displayMessage("Booting...");
  Logger.infoln("Display initialized");

  Logger.infoln("Initializing network");
  WiFiConnection.begin();
  GeneralInformation.displayMessage("IP Address: ");
  GeneralInformation.displayMessage(WiFi.localIP().toString());
  Logger.infoln("Network initialized");

  Logger.infoln("Initializing sensors");
  this->m_oneWire = new OneWire(ONE_WIRE_SENSOR_PIN);

  MQTTPubSubClient *mqttPubSubClient = new MQTTPubSubClient();
  SensorActionMQTT *sensorActionMQTT = new SensorActionMQTT(mqttPubSubClient);
          Serial.println("Program::ctor - sensorActionMQTT address: 0x" + String((uint32_t)sensorActionMQTT, HEX));
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
  this->m_diagnosticButton->tick();
  this->m_lcd->tick();

  unsigned long currentDate = millis();

  if (currentDate - this->m_lastRead > Configuration.getReadInterval()) {
    Logger.infoln("Reading sensors");
    this->m_lastRead = currentDate;

    for (Sensor *sensor : this->m_sensors) {
      sensor->process();
    }
  }
}
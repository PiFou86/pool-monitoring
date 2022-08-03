#include "Program.h"

#include <BluetoothSerial.h>
#include <OneWire.h>

#include "Constantes.h"
#include "Configuration/Configuration.h"
#include "Display/LCD2004Display.h"
#include "Input/Button.h"
#include "Input/ButtonActionBacklightOn.h"
#include "Input/ButtonActionConfiguration.h"
#include "Input/ButtonActionNull.h"
#include "Log/GeneralInformation.h"
#include "Log/LCD2004MessageDisplay.h"
#include "Log/Logger.h"
#include "Network/MQTTPubSubClient.h"
#include "Network/WiFiConnection.h"
#include "Sensor/AnalogADS1115PressureSensor.h"
#include "Sensor/BME280Sensor.h"
#include "Sensor/DS18B20Sensor.h"
#include "Sensor/SensorActionAggregator.h"
#include "Sensor/SensorActionLCD2004.h"
#include "Sensor/SensorActionMQTT.h"
#include "Sensor/SensorActionNull.h"

Program::Program() {
  bool hasBluetooth = true;

  Serial.begin(SERIAL_BAUD);
  Logger.begin(&Serial);
  // #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  //   Logger.infoln(
  //       F("Bluetooth is not enabled! Please run `make menuconfig` to and
  //       enable "
  //         "it"));
  //   hasBluetooth = false;
  // #endif

  // #if !defined(CONFIG_BT_SPP_ENABLED)
  //   Logger.infoln(
  //       F("Bluetooth is not enabled! Please run `make menuconfig` to and
  //       enable "
  //         "it"));
  //   hasBluetooth = false;
  // #endif

  // if (hasBluetooth) {
  //   BluetoothSerial *SerialBT = new BluetoothSerial();
  //   SerialBT->begin("Swimming Pool serial log");

  //   Logger.begin(SerialBT);
  // }

  Logger.infoln("Program started");
  Logger.infoln("Initializing configuration");
  Configuration.begin();
  Logger.infoln(F("Configuration initialized"));

  Logger.infoln(F("Scanning I2C bus for sensors"));
  Configuration_Impl::getI2CAddresses();

  OneWire ow(ONE_WIRE_SENSOR_PIN);
  DallasTemperature DS18B20sensors(&ow);

  DeviceAddress Thermometer;

  Logger.infoln(F("Locating DS18B20 devices..."));
  Logger.info(F("Found "));
  uint8_t deviceCount = DS18B20sensors.getDeviceCount();
  Logger.info(String(deviceCount), false);
  Logger.infoln(F(" devices."), false);
  Logger.infoln(F(""), false);

  Logger.infoln(F("Printing addresses..."));
  std::vector<String> addresses;
  for (uint8_t i = 0; i < deviceCount; i++) {
    DS18B20sensors.getAddress(Thermometer, i);
    String address = Configuration.addressToString(Thermometer, 8);
    Logger.infoln(String(F(" - ")) + address);
    addresses.push_back(address);
  }

  Logger.infoln(F("Initializing display"));
  this->m_lcd = new LCD2004Display();
  GeneralInformation.setMessageDisplay(new LCD2004MessageDisplay(this->m_lcd));
  GeneralInformation.displayMessage(F("Booting..."));
  Logger.infoln(F("Display initialized"));

  Logger.infoln(F("Initializing network"));
  WiFiConnection.begin();
  GeneralInformation.displayMessage(F("IP Address: "));
  GeneralInformation.displayMessage(WiFi.localIP().toString());
  Logger.infoln(F("Network initialized"));

  Logger.infoln(F("Initializing sensors"));
  this->m_oneWire = new OneWire(ONE_WIRE_SENSOR_PIN);

  MQTTPubSubClient *mqttPubSubClient = new MQTTPubSubClient();
  SensorActionMQTT *sensorActionMQTT = new SensorActionMQTT(mqttPubSubClient);
  SensorActionLCD2004 *sensorActionLCD2004 =
      new SensorActionLCD2004(this->m_lcd);
  SensorActionAggregator *sensorActionAggregator = new SensorActionAggregator();
  sensorActionAggregator->addSensorAction(sensorActionMQTT);
  sensorActionAggregator->addSensorAction(sensorActionLCD2004);

  this->m_sensors.push_back(
      new BME280Sensor(F("pool_outdoor"), sensorActionAggregator));
  this->m_sensors.push_back(new DS18B20Sensor(
      F("pool_heater_in"), sensorActionAggregator, this->m_oneWire,
      Configuration.getPoolHeaterInAddress()));
  this->m_sensors.push_back(new DS18B20Sensor(
      F("pool_heater_out"), sensorActionAggregator, this->m_oneWire,
      Configuration.getPoolHeaterOutAddress()));
  this->m_sensors.push_back(new AnalogADS1115PressureSensor(
      F("pool_pressure"), sensorActionAggregator, 0, 0.0f, 30.0f));
  Logger.infoln(F("Sensors initialized"));

  Logger.infoln(F("Initializing inputs"));
  this->m_diagnosticButton = new Button(
      DIAGNOSTIC_PUSH_BUTTON_PIN, new ButtonActionBacklightOn(this->m_lcd),
      new ButtonActionConfiguration());
  Logger.infoln(F("Inputs initialized"));

  Logger.infoln(F("Sending auto-discovery"));
  for (Sensor *sensor : this->m_sensors) {
    sensor->sendAutodiscovery();
  }

  Logger.infoln(F("Auto-discovery sent"));

  this->m_lastRead = 0;
}

void Program::loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      Configuration.executeCommand(this->m_command);
      this->m_command = "";
    } else {
      this->m_command += c;
    }
  }

  this->m_diagnosticButton->tick();
  this->m_lcd->tick();

  unsigned long currentDate = millis();

  if (currentDate - this->m_lastRead > Configuration.getReadInterval()) {
    Logger.infoln(F("Reading sensors"));
    this->m_lastRead = currentDate;

    for (Sensor *sensor : this->m_sensors) {
      sensor->process();
    }
  }
}

// ADS1115

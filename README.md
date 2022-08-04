# Pool monitoring system

This code is a simple pool monitoring system. The system is designed to be run on an ESP32. It can take :

- in / out temperature of the heat pump
- external temperature, pressure and humidity
- water pressure

The device can be configured by a web interface (first launch / WiFi connexion error / long press). An AP is created on the ESP32. You can connect to it with a browser.

The device sends data to an MQTT broker and easily integrate in Home Assistant with MQTT autodiscovery.

It is written in C++ in Platform IO. The first intention was to use an ESP8266 as a microcontroller, but it turned out to be memory limited, so I decided to use an ESP32.

The system is given has is. The code is open source and can be used for personal use.

## Hardware

This project requires an ESP32 as a microcontroller and some peripherals:

- 1 BME280 sensor (I2C) for external temperature, pressure and humidity
- 2 DS18B20 sensors (1 wire) for water in and out temperatures
- 1 ADS1115 ADC (I2C) for  the analog water pressure sensor
- 1 analog water pressure sensor 0-30 PSI (0-4,5V) for the sand filter
- 1 I2C LCD 20x4 to display current information
- 1 push button to activate the LCD backlight (simple press) or launch configuration mode (long press)

![Schematic](img/schema01.png)

## Serial commands

The list of available commands can be obtained by sending the command ```help```.

```
> help
Available commands:
 Configuration:
  infos               | Print configuration infos
  get <key>           | Get the value of a configuration key
  set <key> <value>   | Set configuration value (needs reboot)
  save                | Save configuration
  reboot              | Reboot the device

 Tools:
  scan <deviceType>   | Available device types : i2c, ds18b20
  id                  | Print the device ID

  help
```

Here is some examples of commands:

```
> id
ID: 66666666

> infos
Configuration infos :
---------------------
  poolHeaterInAddress: 28:6a:76:4c:38:43:13:23
  poolHeaterOutAddress: 28:65:16:4b:ab:42:13:43
  mqttServerIP: 192.168.23.123
  mqttServerPort: 1883
  mqttUser: mqttuser
  mqttPassword: Passw0rd
  mqttTopicPrefix: piscine
  debug: 4
---------------------
```

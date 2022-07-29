#pragma once

#include <Arduino.h>

// LOG
#define LOG_LEVEL_ERROR_ONLY 3
#define LOG_LEVEL_WARNING    2
#define LOG_LEVEL_INFO       1

#define LOG_ENABLED          true
#define LOG_LEVEL            LOG_LEVEL_INFO

// Serial
#define SERIAL_BAUD          115000

// Configuration portal
#define CONFIGURATION_PORTAL_SSID      "PoolMonitoringConfiguration"
#define CONFIGURATION_PORTAL_PASSWORD  "Passw0rd"

// Button
#define BUTTON_DEBOUNCE_DELAY          50
#define BUTTON_LONG_PRESS_DELAY        5000

#if ESP8266
    #define DIAGNOSTIC_PUSH_BUTTON_PIN    15   
#elif ESP32
    #define DIAGNOSTIC_PUSH_BUTTON_PIN    16
#endif


// Display 
#define VERTICAL_SCROLL_DELAY           1000
#define BACKLIGHT_DELAY                 10000


// OneWire configuration  GPIO 2 = D4
#define ONE_WIRE_SENSOR_PIN             2
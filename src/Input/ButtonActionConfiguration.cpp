#include "Input/ButtonActionConfiguration.h"

#include "Network/WiFiConnection.h"
#include "Log/Logger.h"

void ButtonActionConfiguration::execute() {
    Logger.infoln(F("Long press : starting WiFi portal"));
    WiFiConnection.startPortal();
}

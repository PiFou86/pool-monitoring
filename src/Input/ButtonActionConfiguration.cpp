#include "Input/ButtonActionConfiguration.h"

#include "Network/WiFiConnection.h"

void ButtonActionConfiguration::execute() {
    WiFiConnection.startPortal();
}

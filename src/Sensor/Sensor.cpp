#include "Sensor/Sensor.h"

Sensor::Sensor(const String &id, SensorAction *sensorAction) : m_id(id), m_sensorAction(sensorAction), m_isReady(false)
{
    ;
}

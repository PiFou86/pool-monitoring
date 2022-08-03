// #include "Sensor/AnalogPressureSensor.h"

// #include <Arduino.h>

// AnalogPressureSensor::AnalogPressureSensor(const String &id,
//                                            SensorAction *sensorAction,
//                                            uint8_t pin, float minPressure,
//                                            float maxPressure)
//     : Sensor(id, sensorAction),
//       m_pin(pin),
//       m_minPressure(minPressure),
//       m_maxPressure(maxPressure) {
//   pinMode(m_pin, INPUT);
//   this->ready();
// }

// void AnalogPressureSensor::readAndInform() {
//   float pressure = analogRead(this->m_pin) / 4095.0 *
//                        (this->m_maxPressure - this->m_minPressure) +
//                    this->m_minPressure;
//   this->dataRead(F("pressure"), pressure, F("PSI"));
// }

// void AnalogPressureSensor::discovery() {
//   this->actionDiscovery(String(F("pressure")), this->getId(), String(F("PSI")));
// }

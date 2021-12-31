#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "config.h"

extern bool sensor_init_ok[NUM_LOX];

void initSensors();
uint16_t readSensor(uint8_t i);
bool sensorActivated(uint8_t i);
void logSensors();

#endif

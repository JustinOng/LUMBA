#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

constexpr uint8_t PIN_SDA = 23;
constexpr uint8_t PIN_SCL = 27;

constexpr uint8_t NUM_LOX = 4;

const uint8_t LOX_ADDRESS[NUM_LOX] = {0x30, 0x31, 0x32, 0x33};
// rst pins for LOX
const uint8_t LOX_SHT[NUM_LOX] = {18, 19, 20, 21};

extern bool sensor_init_ok[NUM_LOX];

void initSensors();
bool sensorActivated(uint8_t i);

#endif

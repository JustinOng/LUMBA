#include "sensors.h"

#include <WebSerial.h>

#include "VL53L0X.h"

struct {
  uint16_t threshold;
  uint16_t activate_debounce;
} config;

bool sensor_init_ok[NUM_LOX] = {false};
VL53L0X lox[NUM_LOX];

void initSensors() {
  // Wire.begin(23, 27);
  Wire.begin(PIN_SDA, PIN_SCL);

  // assert all RST
  for (uint8_t i = 0; i < NUM_LOX; i++) {
    pinMode(LOX_SHT[i], OUTPUT);
    digitalWrite(LOX_SHT[i], LOW);
  }

  delay(10);

  // release RST one at a time and init the sensor

  for (uint8_t i = 0; i < NUM_LOX; i++) {
    digitalWrite(LOX_SHT[i], HIGH);
    delay(10);

    if (!lox[i].init()) {
      Serial.print("Could not detect VL53L0X unit ");
      Serial.println(i);
    } else {
      lox[i].setAddress(LOX_ADDRESS[i]);
      lox[i].startContinuous();
      sensor_init_ok[i] = true;
    }
  }

  config.threshold = 1000;
  config.activate_debounce = 500;
}

bool sensorActivated(uint8_t i, uint16_t min, uint16_t max) {
  static bool pWithinRange[NUM_LOX] = {false};
  static uint32_t last_activate[NUM_LOX] = {0};

  static int32_t acc_delta[NUM_LOX] = {0};
  static uint16_t pReading[NUM_LOX] = {0};

  bool activated = false;

  uint16_t reading = readSensor(i);

  int16_t delta = reading - pReading[i];
  pReading[i] = reading;

  acc_delta[i] = acc_delta[i] * 0.5 + delta;

  if (abs(acc_delta[i]) > 30) {
    return false;
  }

  bool withinRange = reading > min && reading < max;

  if (withinRange && !pWithinRange[i]) {
    if ((millis() - last_activate[i]) > config.activate_debounce) {
      last_activate[i] = millis();

      activated = true;
    }
  }

  pWithinRange[i] = withinRange;

  return activated;
}

void logSensors() {
  Serial.print("LOX: ");
  for (uint8_t i = 0; i < NUM_LOX; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(readSensor(i));
    Serial.print(" ");
  }
  Serial.println();
}

uint16_t readSensor(uint8_t i) {
  static uint32_t last_read[NUM_LOX] = {0};
  static uint16_t last_val[NUM_LOX] = {0};

  constexpr float alpha = 0.1;

  uint16_t &val = last_val[i];

  if (millis() - last_read[i] > 50) {
    uint16_t new_val = lox[i].readRangeContinuousMillimeters();
    last_read[i] = millis();

    if (new_val > 1500) {
      new_val = 1500;
    }
    val = val * (1 - alpha) + new_val * alpha;
  }

  return val;
}

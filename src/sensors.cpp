#include "sensors.h"

#include "VL53L0X.h"

struct {
  uint16_t threshold;
  uint16_t activate_debounce;
} config;

bool sensor_init_ok[NUM_LOX] = {false};
VL53L0X lox[NUM_LOX];

void initSensors() {
  Wire.begin(23, 27);
  // Wire.begin(PIN_SDA, PIN_SCL);

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

bool sensorActivated(uint8_t i) {
  static bool pWithinRange[NUM_LOX] = {false};
  static uint32_t last_activate[NUM_LOX] = {0};

  bool activated = false;

  bool withinRange = lox[i].readRangeContinuousMillimeters() < config.threshold;

  if (withinRange && !pWithinRange[i]) {
    if ((millis() - last_activate[i]) > config.activate_debounce) {
      last_activate[i] = millis();

      activated = true;
    }
  }

  pWithinRange[i] = withinRange;

  return activated;
}

#include "sensors.h"

struct {
  uint16_t threshold;
  uint16_t activate_debounce;
} config;

bool sensor_init_ok[NUM_LOX] = {false};
Adafruit_VL53L0X lox[NUM_LOX];

void initSensors() {
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

    if (!lox[i].begin(LOX_ADDRESS[i])) {
      Serial.print("Could not detect VL53L0X unit ");
      Serial.println(i);
    } else {
      lox[i].startRangeContinuous();
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

  VL53L0X_RangingMeasurementData_t measurement;
  if (lox[i].getSingleRangingMeasurement(&measurement, false) == VL53L0X_ERROR_NONE) {
    bool withinRange = measurement.RangeMilliMeter < config.threshold;

    if (withinRange && !pWithinRange[i]) {
      if ((millis() - last_activate[i]) > config.activate_debounce) {
        last_activate[i] = millis();

        activated = true;
      }
    }

    pWithinRange[i] = withinRange;
  }

  return activated;
}

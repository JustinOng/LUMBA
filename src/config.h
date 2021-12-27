#ifndef CONFIG_H
#define CONFIG_H

#define NUM_LEDS (720 + 70)

#define DATA_PIN 25
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define FPS 60
#define CALC_FPS 60

constexpr uint8_t PIN_LED_J7 = 27;
constexpr uint8_t PIN_LED_J8 = 26;
constexpr uint8_t PIN_LED_J9 = 33;
constexpr uint8_t PIN_LED_J10 = 32;

constexpr uint8_t PIN_SDA = 16;
constexpr uint8_t PIN_SCL = 17;

constexpr uint8_t NUM_LOX = 4;
const uint8_t LOX_ADDRESS[NUM_LOX] = {0x30, 0x31, 0x32, 0x33};
// rst pins for VL53L0X
const uint8_t LOX_SHT[NUM_LOX] = {18, 19, 21, 5};

#endif

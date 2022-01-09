#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <AsyncWebConfig.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <WebSerial.h>
#include <WiFi.h>

#include "config.h"
#include "drawFireworks.h"
#include "drawWaves.h"
#include "dynConfig.h"
#include "helpers.h"
#include "sensors.h"
#include "webserverParams.h"

FASTLED_USING_NAMESPACE

enum {
  PAT_WAVE0 = 0,
  PAT_WAVE1,
  PAT_WAVE2,
  PAT_WAVE3,
  PAT_WAVE4,
  PAT_FIREWORKS = 5,
  PAT_METEORS = 6,
  PAT_STAR_LADDER = 7
};

segment_t segments[] = {
    {.start = 359,
     .end = 0,
     .invert = true},
    {.start = 360,
     .end = 720,
     .invert = true},
    {.start = 850,
     .end = 721,
     .invert = true},
    {.start = 851,
     .end = NUM_LEDS,
     .invert = true}};

segment_t segments_fw[] = {
    {.start = 0,
     .end = 359,
     .invert = true},
    {.start = 720,
     .end = 360,
     .invert = true},
    {.start = 721,
     .end = 850,
     .invert = true},
    {.start = NUM_LEDS,
     .end = 851,
     .invert = true}};

AsyncWebServer server(80);
AsyncWebConfig conf;

dyn_config_t config;

constexpr uint8_t NUM_EFFECTS = 4;

CRGB leds_base[NUM_BASE_LEDS];
// represents actual data written to the LED
CRGB leds[NUM_LEDS];
// buffer for fading
CRGB leds_buf[NUM_LEDS];

void calcHandler();

void readParams() {
  config.brightness = conf.getInt("brightness");
  config.sec_color = strtol(conf.getString("sec_color").c_str() + 1, NULL, 16);

  config.fw_colors[0] = strtol(conf.getString("fw_color_0").c_str() + 1, NULL, 16);
  config.fw_colors[1] = strtol(conf.getString("fw_color_1").c_str() + 1, NULL, 16);
  config.fw_colors[2] = strtol(conf.getString("fw_color_2").c_str() + 1, NULL, 16);
  config.fw_colors[3] = strtol(conf.getString("fw_color_3").c_str() + 1, NULL, 16);
  config.fw_colors[4] = strtol(conf.getString("fw_color_4").c_str() + 1, NULL, 16);

  config.fw_increment = conf.getInt("fw_increment");

  config.move_speed = conf.getInt("move_speed");

  for (uint8_t i = 0; i < 5; i++) {
    config.waves[i].color = strtol(conf.getString("wave_color").c_str() + 1, NULL, 16);

    char buf[32];
    sprintf(buf, "wave%d_speed", i);
    config.waves[i].speed = conf.getInt(buf);
    sprintf(buf, "wave%d_freq", i);
    config.waves[i].freq = conf.getInt(buf);
    sprintf(buf, "wave%d_duty", i);
    config.waves[i].duty = conf.getInt(buf);
  }

  config.base_color = strtol(conf.getString("base_color").c_str() + 1, NULL, 16);

  config.meteors_color = strtol(conf.getString("meteors_color").c_str() + 1, NULL, 16);
  config.meteors_period = conf.getInt("meteors_period");
  config.meteors_speed = conf.getInt("meteors_speed");
  config.meteors_fade = conf.getInt("meteors_fade");
  config.meteors_length = conf.getInt("meteors_length");

  config.sl_color = strtol(conf.getString("sl_color").c_str() + 1, NULL, 16);
  config.sl_led_step = conf.getInt("sl_led_step");
  config.sl_step_time = conf.getInt("sl_step_time");
  config.sl_fade_time = conf.getInt("sl_fade_time");
  config.sl_cycle_time = conf.getInt("sl_cycle_time");
  config.sl_star_len = conf.getInt("sl_star_len");

  config.mode = conf.getValue("mode")[0];
  config.pattern_num = conf.getValue("pattern_num")[0];

  config.eff_caps_color = strtol(conf.getString("eff_caps_color").c_str() + 1, NULL, 16);
  config.eff_caps_dur = conf.getInt("eff_caps_dur");
  config.eff_caps_slew = conf.getInt("eff_caps_slew");

  config.auto_interval_waves = conf.getInt("auto_interval_wave");
  config.auto_interval_fireworks = conf.getInt("auto_interval_fw");
  config.fade_duration = conf.getInt("fade_duration");

  segments[0].start = conf.getInt("led_mid");
  segments[1].start = conf.getInt("led_mid") + 1;
  segments[1].end = conf.getInt("led_arc_corner");
  segments[2].start = conf.getInt("led_arc_mid");
  segments[2].end = conf.getInt("led_arc_corner") + 1;
  segments[3].start = conf.getInt("led_arc_mid") + 1;
  segments[3].end = min(conf.getInt("led_end"), NUM_LEDS);

  for (uint8_t i = 0; i < 4; i++) {
    segments_fw[i].start = segments[i].end;
    segments_fw[i].end = segments[i].start;
  }

  config.lox_min[0] = conf.getInt("lox_min_0");
  config.lox_max[0] = conf.getInt("lox_max_0");
  config.lox_min[1] = conf.getInt("lox_min_1");
  config.lox_max[1] = conf.getInt("lox_max_1");
  config.lox_min[2] = conf.getInt("lox_min_2");
  config.lox_max[2] = conf.getInt("lox_max_2");
  config.lox_min[3] = conf.getInt("lox_min_3");
  config.lox_max[3] = conf.getInt("lox_max_3");

  config.led_pos_display = conf.getBool("led_pos_display");
}

void handleRoot(AsyncWebServerRequest* request) {
  conf.handleFormRequest(request);
  if (request->hasParam("SAVE", true)) {
    uint8_t cnt = conf.getCount();
    Serial.println("Saving configuration");
    for (uint8_t i = 0; i < cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(" = ");
      Serial.println(conf.values[i]);
    }
    readParams();
  }
}

hw_timer_t* timer = NULL;

uint32_t eff_caps_start = -1;

typedef struct {
  uint8_t delta;

  uint8_t caps_palette_index;

  uint16_t meteors_offset;
} runtime_data_t;

runtime_data_t runtime_data;

SemaphoreHandle_t param_access;

uint16_t lox_readings[NUM_LOX] = {0};
void setup() {
  Serial.begin(115200);

  conf.setDescription(params);
  conf.readConfig();

  boolean connected = false;
  WiFi.mode(WIFI_STA);
  if (conf.values[0] != "") {
    Serial.print("Connecting to SSID = ");
    Serial.println(conf.values[0]);
    WiFi.begin(conf.values[0].c_str(), conf.values[1].c_str());
    uint8_t cnt = 0;
    while ((WiFi.status() != WL_CONNECTED) && (cnt < 20)) {
      delay(500);
      Serial.print(".");
      cnt++;
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("IP = ");
      Serial.println(WiFi.localIP());
      connected = true;
    }
  }

  if (!connected) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(conf.getApName(), "long password");
  }

  server.on("/", handleRoot);
  server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest* request) {
    constexpr uint8_t LEN_BUF = 128;
    char buf[LEN_BUF];
    snprintf(buf, LEN_BUF, "Sensor 0: %d<br/>Sensor 1: %d<br/>Sensor 2: %d<br/>Sensor 3: %d<br/>", lox_readings[0], lox_readings[1], lox_readings[2], lox_readings[3]);
    request->send(200, "text/html", buf);
  });

  AsyncElegantOTA.begin(&server);
  WebSerial.begin(&server);
  server.begin();
  readParams();

  FastLED.addLeds<LED_TYPE, PIN_LED_J7, COLOR_ORDER>(leds_base, NUM_BASE_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_J10, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  initSensors();

  param_access = xSemaphoreCreateMutex();
  assert(param_access != nullptr);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &calcHandler, true);
  timerAlarmWrite(timer, 1000000 / CALC_FPS, true);
  timerAlarmEnable(timer);

  WebSerial.println("Hello");
}

// auto mode: controls which wave state/fireworks is being displayed
// manual mode, param "pattern_num" overrides this
uint8_t active_pattern = 0;
uint32_t last_pattern_change = 0;
constexpr uint8_t MAX_PATTERN = 7;

typedef enum {
  FADE_IDLE,
  FADE_OUT,
  FADE_OUT_DONE,
} state_t;

state_t fade_state = FADE_IDLE;

void loop() {
  static uint32_t fade_out_start = -1;
  fill_solid(leds_base, NUM_BASE_LEDS, config.base_color);

  runtime_data_t data;
  xSemaphoreTake(param_access, portMAX_DELAY);
  memcpy(&data, &runtime_data, sizeof(runtime_data_t));
  xSemaphoreGive(param_access);

  static uint32_t last_sensor_read = 0;
  if (millis() - last_sensor_read > 50) {
    last_sensor_read = millis();

    for (uint8_t i = 0; i < NUM_LOX; i++) {
      if (sensorActivated(i, config.lox_min[i], config.lox_max[i])) {
        WebSerial.print((uint32_t)millis());
        WebSerial.print("Triggered sensor ");
        WebSerial.println(i);

        eff_caps_start = millis();
      }

      lox_readings[i] = readSensor(i);
    }
  }

  if (Serial.available() && Serial.read() == 'a') {
    Serial.print("Triggered ");
    Serial.println(data.caps_palette_index);
    eff_caps_start = millis();
  }

  // static uint32_t last_sensor_debug_read = 0;
  // if (millis() - last_sensor_debug_read > 500) {
  //   WebSerial.println(data.caps_palette_index);
  // }

  uint32_t start = micros();
  FastLED.setBrightness(config.brightness);

  if (config.mode == '0') {
    // manual
    // convert ASCII digit into number
    uint8_t new_active_pattern = config.pattern_num - '0';

    if (active_pattern != new_active_pattern) {
      last_pattern_change = millis();
      active_pattern = new_active_pattern;

      if (active_pattern > 4) {
        // wipe LED buffer if next playing non-wave
        fill_solid(leds, NUM_LEDS, CRGB::Black);
      }
    }
  } else {
    // auto
    if (
        ((active_pattern <= 4) && (millis() - last_pattern_change) > (config.auto_interval_waves * 1000)) ||
        ((active_pattern > 4) && (millis() - last_pattern_change) > (config.auto_interval_fireworks * 1000))) {
      static uint8_t last_delta = 0;

      if ((active_pattern == PAT_WAVE4 || active_pattern == PAT_FIREWORKS || active_pattern == PAT_METEORS || active_pattern == PAT_STAR_LADDER) && fade_state != FADE_OUT_DONE) {
        if (fade_state != FADE_OUT) {
          fade_state = FADE_OUT;
          Serial.println("Starting fade out");
          fade_out_start = millis();
        }
      } else {
        fade_state = FADE_IDLE;
        // if we are transitioning between waves, change only at the end of the period
        // so that we don't have an abrupt transition
        if (active_pattern >= 4 || data.delta < last_delta) {
          last_pattern_change = millis();

          active_pattern++;

          if (active_pattern > MAX_PATTERN) {
            active_pattern = 0;
          }

          Serial.print("Change to pattern ");
          Serial.println(active_pattern);
        }

        last_delta = data.delta;
      }
    }
  }

  CRGB* buf = leds;

  static uint8_t pPattern = -1;

  switch (active_pattern) {
    case PAT_WAVE0:
    case PAT_WAVE1:
    case PAT_WAVE2:
    case PAT_WAVE3:
    case PAT_WAVE4: {
      static uint8_t last_run_delta = 0;

      // because computation runs at a fixed frame rate while the drawing rate is dependent
      // on the number of LEDs, we play potential catch up here if delta has been incremented
      // more than one time
      while (data.delta != last_run_delta) {
        for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
          drawWaves(buf, config.waves[active_pattern], config.eff_caps_color, data.caps_palette_index, data.delta, segments[i], fade_state != FADE_OUT);
        }

        last_run_delta++;
      }

      break;
    }
    case PAT_METEORS: {
      static uint8_t pOffset = 0;

      if (pPattern != active_pattern) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
      }

      while (pOffset != data.meteors_offset) {
        for (uint8_t i = 0; i < sizeof(segments_fw) / sizeof(segment_t); i++) {
          segment_t& segment = segments_fw[i];
          uint16_t segment_length = getSegmentLength(segment);
          // shift all LEDs one space next
          for (int16_t i = segment_length - 1; i > 0; i--) {
            leds[getPixelIndex(i, segment)] = leds[getPixelIndex(i - 1, segment)];
          }

          // default the first LED to a dimmed verison of the second LED
          leds[getPixelIndex(0, segment)] = leds[getPixelIndex(1, segment)];
          leds[getPixelIndex(0, segment)].fadeToBlackBy(config.meteors_fade);

          // if we're at the start of the period, introduce new meteor
          if ((pOffset % config.meteors_period) < config.meteors_length && fade_state != FADE_OUT) {
            leds[getPixelIndex(0, segment)] = CRGB(config.meteors_color);
          }
        }
        pOffset++;
      }
      break;
    }
    case PAT_STAR_LADDER: {
      uint32_t pattern_time = (millis() - last_pattern_change) % config.sl_cycle_time;
      bool fade_in = ((millis() - last_pattern_change) % (config.sl_cycle_time * 2)) < config.sl_cycle_time;

      static uint16_t pBase_pos = 0;
      uint16_t base_pos = (pattern_time / config.sl_step_time) * config.sl_led_step;
      for (uint8_t i = 0; i < sizeof(segments_fw) / sizeof(segment_t); i++) {
        segment_t& segment = segments_fw[i];
        uint16_t star_pos = base_pos;
        uint32_t star_time = pattern_time % config.sl_step_time;

        for (uint8_t u = 0; u < config.sl_star_len; u++) {
          if (fade_in) {
            if (!withinSegment(star_pos, segment)) {
              break;
            }
            leds[getPixelIndex(star_pos, segment)] = CRGB(config.sl_color);

            if (fade_state != FADE_OUT) {
              if (star_time < config.sl_fade_time) {
                // fade in star if within sl_fade_time
                leds[getPixelIndex(star_pos, segment)].fadeToBlackBy(map(star_time, 0, config.sl_fade_time, 255, 0));
              }

              // ensure that previous LED has been fully lit
              if (star_pos >= config.sl_led_step) {
                leds[getPixelIndex(star_pos - config.sl_led_step, segment)] = CRGB(config.sl_color);
              }
            }

            // handle edge case where last star may not be faded out
            if (pBase_pos > 0 && base_pos == 0) {
              if (withinSegment(pBase_pos + u, segment)) {
                leds[getPixelIndex(pBase_pos + u, segment)] = CRGB::Black;
              }
            }
          } else {
            if (withinSegment(star_pos, segment)) {
              if (fade_state != FADE_OUT) {
                leds[getPixelIndex(star_pos, segment)] = CRGB(config.sl_color);
              }
              if (star_time < config.sl_fade_time) {
                // fade out star if within sl_fade_time
                leds[getPixelIndex(star_pos, segment)].fadeToBlackBy(map(star_time, 0, config.sl_fade_time, 0, 255));
              } else {
                leds[getPixelIndex(star_pos, segment)] = CRGB::Black;
              }
            }

            // ensure that previous LED has been fully dimmed
            if (star_pos >= config.sl_led_step) {
              uint16_t star_pos_prior = star_pos - config.sl_led_step;
              if (withinSegment(star_pos_prior, segment)) {
                leds[getPixelIndex(star_pos_prior, segment)] = CRGB::Black;
              }
            }
          }
          star_pos++;
        }
      }

      pBase_pos = base_pos;
      break;
    }
    case PAT_FIREWORKS: {
      CRGBPalette16 fw_palette;

#define MAP_PALETTE(start, end, color_index)       \
  for (uint8_t i = start; i < end; i++) {          \
    fw_palette[i] = config.fw_colors[color_index]; \
  }
      MAP_PALETTE(0, 2, 0)
      MAP_PALETTE(3, 5, 1)
      MAP_PALETTE(6, 8, 2)
      MAP_PALETTE(8, 11, 3)
      MAP_PALETTE(12, 15, 4)

      for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
        drawFireworks(buf, fw_palette, segments[i].start, segments[i].end, data.delta, !segments[i].invert);
      }
      break;
    }
    case 9:
      fill_solid(leds, NUM_LEDS, config.sec_color);
      break;
  }

  pPattern = active_pattern;

  if (fade_state == FADE_OUT) {
    if (millis() - fade_out_start < config.fade_duration) {
      int32_t delta = millis() - fade_out_start;
      // fade out
      uint8_t fade_amount = map(delta, 0, config.fade_duration, 0, 255);
      if (delta < config.fade_duration) {
        fadeToBlackBy(leds, NUM_LEDS, fade_amount);
      } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
      }
    } else {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      fade_state = FADE_OUT_DONE;
    }
  }

  if ((active_pattern == PAT_WAVE0 || active_pattern == PAT_FIREWORKS) && (millis() - last_pattern_change) < config.fade_duration) {
    // fade in
    fadeToBlackBy(leds, NUM_LEDS, map(millis() - last_pattern_change, 0, config.fade_duration, 255, 0));
  }

  EVERY_N_SECONDS(10) {
    Serial.print("Calculation time: ");
    Serial.print(micros() - start);
    Serial.println("us");
  }

  if (config.led_pos_display) {
    if (millis() % 1000 < 500) {
      leds[segments[0].start] = CRGB::Red;
      leds[segments[0].start + 1] = CRGB::Black;

      leds[segments[1].end] = CRGB::Green;
      leds[segments[1].end + 1] = CRGB::Black;

      leds[segments[2].start] = CRGB::Blue;
      leds[segments[2].start + 1] = CRGB::Black;

      leds[segments[3].end] = CRGB::Yellow;
    } else {
      leds[segments[0].start] = CRGB::Black;
      leds[segments[0].start + 1] = CRGB::Red;

      leds[segments[1].end] = CRGB::Black;
      leds[segments[1].end + 1] = CRGB::Green;

      leds[segments[2].start] = CRGB::Black;
      leds[segments[2].start + 1] = CRGB::Blue;

      leds[segments[3].end] = CRGB::Black;
    }
  }

  FastLED.show();
  FastLED.delay(1000 / FPS);
}

void IRAM_ATTR calcHandler() {
  // 16 bit so we can do fractional increases, take the upper 8 bits for actual delta
  static uint16_t delta_shadow = 0;
  static int16_t caps_shadow = 0;
  static uint16_t meteors_offset_shadow = 0;

  xSemaphoreTakeFromISR(param_access, NULL);
  if (active_pattern < 5) {
    delta_shadow += config.waves[active_pattern].speed;
  } else {
    delta_shadow += config.move_speed;
  }

  runtime_data.delta = delta_shadow >> 8;

  if ((millis() - eff_caps_start) < config.eff_caps_dur) {
    if (caps_shadow < 255) {
      caps_shadow += config.eff_caps_slew;
    }
  } else {
    if (caps_shadow > 0) {
      caps_shadow -= config.eff_caps_slew;
    }
  }

  if (caps_shadow > 255) {
    runtime_data.caps_palette_index = 255;
  } else if (caps_shadow < 0) {
    runtime_data.caps_palette_index = 0;
  } else {
    runtime_data.caps_palette_index = caps_shadow;
  }

  meteors_offset_shadow += config.meteors_speed;
  runtime_data.meteors_offset = meteors_offset_shadow >> 8;
  xSemaphoreGive(param_access);
}

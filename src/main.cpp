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

// this defines the segments that the star ladder effect will be drawn on
// note: invert is ignored
segment_t segments_star_ladder[] = {
    {.start = 359,
     .end = 0},
    {.start = 360,
     .end = 720}};

constexpr uint8_t NUM_SEGMENTS_STAR_LADDER = sizeof(segments_star_ladder) / sizeof(segment_t);

// this defines the segments that the line effect will be drawn on
// note: invert is ignored
segment_t segments_line[] = {
    {.start = 359,
     .end = 0},
    {.start = 360,
     .end = 720}};

constexpr uint8_t NUM_SEGMENTS_LINE = sizeof(segments_line) / sizeof(segment_t);

// this defines the segments that the solid line effect will be drawn on
// note: invert is ignored
segment_t segments_sline[] = {
    {.start = 359,
     .end = 0},
    {.start = 360,
     .end = 720}};

constexpr uint8_t NUM_SEGMENTS_SLINE = sizeof(segments_sline) / sizeof(segment_t);

AsyncWebServer server(80);
AsyncWebConfig conf;

dyn_config_t config;

constexpr uint8_t NUM_EFFECTS = 4;

CRGB leds_base[NUM_BASE_LEDS];
// represents actual data written to the LED
CRGB leds[NUM_LEDS];
// used to track effects ruinning
CRGB leds_effect[NUM_EFFECTS][NUM_LEDS];
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

  config.mode = conf.getValue("mode")[0];
  config.pattern_num = conf.getValue("pattern_num")[0];
  config.effect_num = conf.getValue("effect_num")[0];

  config.eff_sl_color = strtol(conf.getString("eff_sl_color").c_str() + 1, NULL, 16);
  config.eff_sl_step = conf.getInt("eff_sl_step");
  config.eff_sl_length = conf.getInt("eff_sl_length");
  config.eff_sl_interval = conf.getInt("eff_sl_interval");
  config.eff_sl_fade = conf.getInt("eff_sl_fade");

  config.eff_rs_color = strtol(conf.getString("eff_rs_color").c_str() + 1, NULL, 16);
  config.eff_rs_duration = conf.getInt("eff_rs_duration");
  config.eff_rs_chance = conf.getInt("eff_rs_chance");
  config.eff_rs_fade = conf.getInt("eff_rs_fade");
  config.eff_rs_length = conf.getInt("eff_rs_length");

  config.eff_line_color = strtol(conf.getString("eff_line_color").c_str() + 1, NULL, 16);
  config.eff_line_speed = conf.getInt("eff_line_speed");
  config.eff_line_duration = conf.getInt("eff_line_duration");
  config.eff_line_fade_dur = conf.getInt("eff_line_fade_dur");
  config.eff_line_period = conf.getInt("eff_line_period");
  config.eff_line_duty = conf.getInt("eff_line_duty");

  config.eff_sline_color = strtol(conf.getString("eff_sline_color").c_str() + 1, NULL, 16);
  config.eff_sline_speed = conf.getInt("eff_sline_speed");
  config.eff_sline_fade = conf.getInt("eff_sline_fade");

  config.eff_caps_color = strtol(conf.getString("eff_caps_color").c_str() + 1, NULL, 16);
  config.eff_caps_dur = conf.getInt("eff_caps_dur");
  config.eff_caps_slew = conf.getInt("eff_caps_slew");

  config.auto_interval = conf.getInt("auto_interval");
  config.fade_duration = conf.getInt("fade_duration");
  config.fade_blend = conf.getInt("fade_blend");

  config.patt_triggers[0] = conf.getValue("patt_trig_0")[0] - '0';
  config.patt_triggers[1] = conf.getValue("patt_trig_1")[0] - '0';
  config.patt_triggers[2] = conf.getValue("patt_trig_2")[0] - '0';
  config.patt_triggers[3] = conf.getValue("patt_trig_3")[0] - '0';

  segments[0].start = conf.getInt("led_mid");
  segments[1].start = conf.getInt("led_mid") + 1;
  segments[1].end = conf.getInt("led_arc_corner");
  segments[2].start = conf.getInt("led_arc_mid");
  segments[2].end = conf.getInt("led_arc_corner") + 1;
  segments[3].start = conf.getInt("led_arc_mid") + 1;

  segments_star_ladder[0].start = segments[0].start;
  segments_star_ladder[1].start = segments[1].start;
  segments_star_ladder[1].end = segments[1].end;

  segments_line[0].start = segments[0].start;
  segments_line[1].start = segments[1].start;
  segments_line[1].end = segments[1].end;

  segments_sline[0].start = segments[0].start;
  segments_sline[1].start = segments[1].start;
  segments_sline[1].end = segments[1].end;

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
  uint16_t line_pos;
  uint16_t sline_pos;

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
constexpr uint8_t MAX_PATTERN = 5;

void loop() {
  // index at which to next draw stars
  static int16_t star_ladder_indexes[NUM_SEGMENTS_STAR_LADDER] = {-1};
  // how long more to draw random stars
  static uint32_t random_stars_start_time = -1;
  static uint32_t line_start_time = -1;
  static uint32_t sline_start_time = -1;

  fill_solid(leds_base, NUM_BASE_LEDS, config.base_color);

  runtime_data_t data;
  xSemaphoreTake(param_access, portMAX_DELAY);
  memcpy(&data, &runtime_data, sizeof(runtime_data_t));
  xSemaphoreGive(param_access);

  static uint32_t last_sensor_read = 0;
  if (millis() - last_sensor_read > 50) {
    last_sensor_read = millis();

    for (uint8_t i = 0; i < NUM_LOX; i++) {
      if (sensorActivated(config.patt_triggers[i], config.lox_min[i], config.lox_max[i])) {
        WebSerial.print("Triggered sensor ");
        WebSerial.println(i);

        eff_caps_start = millis();
      }

      lox_readings[i] = readSensor(i);
    }
  }

  static uint32_t last_sensor_debug_read = 0;
  if (millis() - last_sensor_debug_read > 500) {
    logSensors();
  }

  uint32_t start = micros();
  FastLED.setBrightness(config.brightness);

  if (config.mode == '0') {
    // manual
    // convert ASCII digit into number
    active_pattern = config.pattern_num - '0';
  } else {
    // auto
    if ((millis() - last_pattern_change) > (config.auto_interval * 1000)) {
      static uint8_t last_delta = 0;

      // if we are transitioning between waves, change only at the end of the period
      // so that we don't have an abrupt transition
      if (active_pattern == 4 || data.delta < last_delta) {
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

  CRGB* buf;
  bool fade = ((millis() - last_pattern_change) < config.fade_duration) && (active_pattern == 5 || active_pattern == 0);
  if (fade) {
    buf = leds_buf;
  } else {
    buf = leds;
  }

  switch (active_pattern) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4: {
      static uint8_t last_run_delta = 0;

      // because computation runs at a fixed frame rate while the drawing rate is dependent
      // on the number of LEDs, we play potential catch up here if delta has been incremented
      // more than one time
      while (data.delta != last_run_delta) {
        for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
          drawWaves(buf, config.waves[active_pattern], config.eff_caps_color, data.caps_palette_index, data.delta, segments[i]);
        }

        last_run_delta++;
      }
    }
    case 5: {
      for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
        segment_t& segment = segments[i];
        uint16_t segment_length = getSegmentLength(segment);
        // shift all LEDs one space next
        for (int16_t i = segment_length - 1; i > 0; i--) {
          leds[getPixelIndex(i, segment)] = leds[getPixelIndex(i - 1, segment)];
        }

        // default the first LED to a dimmed verison of the second LED
        leds[getPixelIndex(0, segment)] = leds[getPixelIndex(1, segment)];
        leds[getPixelIndex(0, segment)].fadeToBlackBy(config.meteors_fade);

        // if we're at the start of the period, introduce new meteor
        if (data.meteors_offset < config.meteors_length) {
          leds[getPixelIndex(0, segment)] = CRGB(config.meteors_color);
        }
      }
      break;
    }
    case 6: {
      break;
    }
    case 7: {
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

  if (fade) {
    memcpy((uint8_t*)leds, (uint8_t*)leds_buf, NUM_LEDS * sizeof(CRGB));

    int32_t delta = millis() - last_pattern_change;
    if (delta > 0 && delta < (config.fade_duration / 2)) {
      // fade out
      fadeToBlackBy(leds, NUM_LEDS, map(delta, 0, config.fade_duration / 2, 0, 255));
    } else if (delta >= (config.fade_duration / 2) && delta < config.fade_duration) {
      // fade in
      fadeToBlackBy(leds, NUM_LEDS, map(delta, config.fade_duration / 2, config.fade_duration, 255, 0));
    }
  }

  EVERY_N_SECONDS(10) {
    Serial.print("Calculation time: ");
    Serial.print(micros() - start);
    Serial.println("us");
  }

  if (config.led_pos_display) {
    if (millis() % 1000 < 500) {
      leds[segments[0].start] = CRGB::Red;
      leds[segments[1].end] = CRGB::Green;
      leds[segments[2].start] = CRGB::Blue;
    } else {
      leds[segments[0].start] = CRGB::Black;
      leds[segments[1].end] = CRGB::Black;
      leds[segments[2].start] = CRGB::Black;
    }
  }

  FastLED.show();
  FastLED.delay(1000 / FPS);
}

void IRAM_ATTR calcHandler() {
  // 16 bit so we can do fractional increases, take the upper 8 bits for actual delta
  static uint16_t delta_shadow = 0;
  // static uint16_t line_pos_shadow = 0;
  static uint32_t sline_pos_shadow = 0;
  static int32_t caps_shadow = 0;
  static uint32_t meteors_offset_shadow = 0;

  xSemaphoreTakeFromISR(param_access, NULL);
  if (active_pattern < 5) {
    delta_shadow += config.waves[active_pattern].speed;
  } else {
    delta_shadow += config.move_speed;
  }

  runtime_data.delta = delta_shadow >> 8;

  // line_pos_shadow += config.eff_line_speed;
  runtime_data.line_pos += 1;

  sline_pos_shadow += config.eff_sline_speed;
  runtime_data.sline_pos = sline_pos_shadow >> 8;

  if ((millis() - eff_caps_start) < config.eff_caps_dur) {
    caps_shadow = max(caps_shadow + config.eff_caps_slew, 65535);
  } else {
    caps_shadow = min(caps_shadow - config.eff_caps_slew, 0);
  }
  runtime_data.caps_palette_index = caps_shadow >> 8;

  meteors_offset_shadow += config.meteors_speed;
  runtime_data.meteors_offset = meteors_offset_shadow >> 8;
  if (runtime_data.meteors_offset >= config.meteors_period) {
    runtime_data.meteors_offset = 0;
    config.meteors_period = 0;
  }
  xSemaphoreGive(param_access);
}

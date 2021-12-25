#include <Arduino.h>
#include <AsyncWebConfig.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

#include "sensors.h"

FASTLED_USING_NAMESPACE

#define NUM_LEDS (720 + 70)

typedef struct {
  uint16_t start;
  uint16_t end;
  bool invert;
} segment_t;

segment_t segments[] = {
    {.start = 44,
     .end = 0,
     .invert = true},
    {.start = 45,
     .end = 90,
     .invert = true}};

#define DATA_PIN 25
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB

#define FPS 60
#define CALC_FPS 60

String params =
    "["
    "{"
    "'name':'ssid',"
    "'label':'Name des WLAN',"
    "'type':" +
    String(INPUTTEXT) +
    ","
    "'default':''"
    "},"
    "{"
    "'name':'pwd',"
    "'label':'WLAN Passwort',"
    "'type':" +
    String(INPUTPASSWORD) +
    ","
    "'default':''"
    "},"
    "{"
    "'name':'fps',"
    "'label':'FPS',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,"
    "'default':'60'"
    "},"
    "{"
    "'name':'color',"
    "'label':'Base Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'sec_color',"
    "'label':'Secondary Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#00ff00'"
    "},"
    "{"
    "'name':'brightness',"
    "'label':'Brightness',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':255,"
    "'default':'60'"
    "},"
    "{"
    "'name':'move_speed',"
    "'label':'Move Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave_freq',"
    "'label':'Wave Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave_duty',"
    "'label':'Wave Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'fw_color_0',"
    "'label':'Fireworks Color 0',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#cbb1d6'"
    "},"
    "{"
    "'name':'fw_color_1',"
    "'label':'Fireworks Color 1',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fecf03'"
    "},"
    "{"
    "'name':'fw_color_2',"
    "'label':'Fireworks Color 2',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#faa564'"
    "},"
    "{"
    "'name':'fw_color_3',"
    "'label':'Fireworks Color 3',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fef7ef'"
    "},"
    "{"
    "'name':'fw_color_4',"
    "'label':'Fireworks Color 4',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fee9f0'"
    "},"
    "{"
    "'name':'fw_increment',"
    "'label':'Fireworks Width Scaling',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'pattern_num',"
    "'label':'Pattern',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Wave'},"
    "{'v':'1','l':'Fireworks'}],"
    "'default':'0'"
    "},"
    "{"
    "'name':'param1',"
    "'label':'Parameter1',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':65535,"
    "'default':'60'"
    "},"
    "{"
    "'name':'param2',"
    "'label':'Parameter2',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':65535,"
    "'default':'60'"
    "},"
    "{"
    "'name':'param3',"
    "'label':'Parameter3',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':255,"
    "'default':'60'"
    "}"
    "]";

AsyncWebServer server(80);
AsyncWebConfig conf;

typedef struct params {
  char num;
  uint16_t fps;
  uint8_t brightness;
  uint32_t base_color;
  uint32_t sec_color;
  uint16_t move_speed;
  float wave_freq;
  int8_t wave_duty;
  uint32_t fw_colors[5];
  uint8_t fw_increment;
  uint16_t param1;
  uint16_t param2;
  uint16_t param3;
} pattern_config_t;

pattern_config_t pattern;

CRGB leds[NUM_LEDS];
CRGB overlay_leds[NUM_LEDS];

void calcHandler();

void readParams() {
  pattern.fps = conf.getInt("fps");
  pattern.brightness = conf.getInt("brightness");
  pattern.base_color = strtol(conf.getString("color").c_str() + 1, NULL, 16);
  pattern.sec_color = strtol(conf.getString("sec_color").c_str() + 1, NULL, 16);

  pattern.fw_colors[0] = strtol(conf.getString("fw_color_0").c_str() + 1, NULL, 16);
  pattern.fw_colors[1] = strtol(conf.getString("fw_color_1").c_str() + 1, NULL, 16);
  pattern.fw_colors[2] = strtol(conf.getString("fw_color_2").c_str() + 1, NULL, 16);
  pattern.fw_colors[3] = strtol(conf.getString("fw_color_3").c_str() + 1, NULL, 16);
  pattern.fw_colors[4] = strtol(conf.getString("fw_color_4").c_str() + 1, NULL, 16);

  pattern.fw_increment = conf.getInt("fw_increment");

  pattern.move_speed = conf.getInt("move_speed");
  pattern.wave_freq = conf.getFloat("wave_freq");
  pattern.wave_duty = conf.getInt("wave_duty");

  pattern.num = conf.getValue("pattern_num")[0];

  pattern.param1 = conf.getInt("param1");
  pattern.param2 = conf.getInt("param2");
  pattern.param3 = conf.getInt("param3");
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

typedef struct {
  uint8_t delta;
  uint8_t line_length;
  uint16_t star_index;
} runtime_data_t;

runtime_data_t runtime_data;

SemaphoreHandle_t param_access;

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
    WiFi.softAP("Sail1", "long password");
  }

  server.on("/", handleRoot);
  server.begin();
  readParams();

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  initSensors();

  param_access = xSemaphoreCreateMutex();
  assert(param_access != nullptr);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &calcHandler, true);
  timerAlarmWrite(timer, 1000000 / CALC_FPS, true);
  timerAlarmEnable(timer);
}

void drawSegment(uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  CRGBPalette16 p = CRGB(pattern.base_color);
  uint8_t pos = delta;
  uint16_t i = start;
  while (i != end) {
    // triwave8 creates the variations, wave_freq scales frequency (same scaling as how sine works)
    // delta shifts the pattern

    if (!invert) {
      pos++;
    } else {
      pos--;
    }

    uint8_t base = triwave8(pattern.wave_freq * pos);
    // Shift up/down based on wave_duty. Rescale it to max 255
    int16_t scaled = (base + pattern.wave_duty) * 255 / (255 + pattern.wave_duty);

    uint8_t bri = 0;
    if (scaled >= 0 && scaled <= 255) {
      bri = scaled;
    } else if (scaled > 255) {
      bri = 255;
    }

    leds[i] = ColorFromPalette(p, i, bri, LINEARBLEND);

    if (start < end) {
      i++;
    } else {
      i--;
    }
  }
}

void loop() {
  runtime_data_t data;
  xSemaphoreTake(param_access, portMAX_DELAY);
  memcpy(&data, &runtime_data, sizeof(runtime_data_t));
  xSemaphoreGive(param_access);

  uint32_t start = micros();
  FastLED.setBrightness(pattern.brightness);

  if (pattern.num == '0') {
    for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
      drawSegment(segments[i].start, segments[i].end, data.delta, segments[i].invert);
    }

    // CRGB star_color = CRGB(pattern.sec_color);
    // static uint32_t last_change = 0;

    // if (millis() - last_change > pattern.param2) {
    //   last_change = millis();
    //   if (star_index < NUM_LEDS) {
    //     for (uint16_t i = star_index; i < (star_index + pattern.param3); i++) {
    //       if (i < (NUM_LEDS - 1)) {
    //         overlay_leds[i] = star_color;
    //       }
    //     }
    //     star_index += pattern.param1;
    //   }
    // }

    fadeToBlackBy(overlay_leds, NUM_LEDS, 32);

    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      leds[i] += overlay_leds[i];
    }
  } else if (pattern.num == '1') {
    CRGBPalette16 fw_palette;

#define MAP_PALETTE(start, end, color_index)        \
  for (uint8_t i = start; i < end; i++) {           \
    fw_palette[i] = pattern.fw_colors[color_index]; \
  }
    MAP_PALETTE(0, 2, 0)
    MAP_PALETTE(3, 5, 1)
    MAP_PALETTE(6, 8, 2)
    MAP_PALETTE(8, 11, 3)
    MAP_PALETTE(12, 15, 4)

    for (uint8_t i = 0; i < sizeof(segments) / sizeof(segment_t); i++) {
      uint8_t pos = data.delta;
      uint16_t u = segments[i].start;
      while (u != segments[i].end) {
        if (!segments[i].invert) {
          pos += pattern.fw_increment;
        } else {
          pos -= pattern.fw_increment;
        }

        leds[u] = ColorFromPalette(fw_palette, pos, 255, LINEARBLEND);

        if (segments[i].start < segments[i].end) {
          u++;
        } else {
          u--;
        }
      }
    }
  }

  EVERY_N_SECONDS(10) {
    Serial.print("Calculation time: ");
    Serial.print(micros() - start);
    Serial.println("us");
  }

  FastLED.show();
  FastLED.delay(1000 / FPS);
}

void IRAM_ATTR calcHandler() {
  xSemaphoreTakeFromISR(param_access, NULL);
  runtime_data.delta += 1;
  xSemaphoreGive(param_access);
}

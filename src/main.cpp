#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <AsyncWebConfig.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 18
#define SHT_LOX2 19

#define NUM_LEDS (720 + 70)
#define NUM_MIDDLE (360 + 12)

#define DATA_PIN 25
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB

#define FPS 60
#define CALC_FPS 30

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
    "'name':'pattern_num',"
    "'label':'Pattern',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Sine Wave'},"
    "{'v':'1','l':'Random'}],"
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

class RandomChange {
 public:
  RandomChange(uint8_t _delta) : RandomChange(_delta, 0, 255) {}
  RandomChange(uint8_t _delta, uint8_t _min, uint8_t _max) {
    delta = _delta;
    min = _min;
    max = _max;
  }

  uint8_t get() {
    if (forward) {
      c += random8(delta);
      if (c > max) {
        c = max;
        forward = false;
      }
    } else {
      c -= random8(delta);
      if (c < min) {
        c = min;
        forward = true;
      }
    }

    return c;
  }

 private:
  uint8_t delta, min, max;
  int16_t c = 0;
  bool forward = true;
};

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
  uint16_t param1;
  uint16_t param2;
  uint16_t param3;
} pattern_config_t;

pattern_config_t pattern;

CRGB leds[NUM_LEDS];
CRGB overlay_leds[NUM_LEDS];
CRGBPalette16 vary_palette;

RandomChange change_palette(pattern.param1);
RandomChange change_brightness(pattern.param2, 50, 255);

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

void readParams() {
  pattern.fps = conf.getInt("fps");
  pattern.brightness = conf.getInt("brightness");
  pattern.base_color = strtol(conf.getString("color").c_str() + 1, NULL, 16);
  pattern.sec_color = strtol(conf.getString("sec_color").c_str() + 1, NULL, 16);
  pattern.move_speed = conf.getInt("move_speed");
  pattern.wave_freq = conf.getFloat("wave_freq");
  pattern.wave_duty = conf.getInt("wave_duty");

  pattern.num = conf.getValue("pattern_num")[0];

  pattern.param1 = conf.getInt("param1");
  pattern.param2 = conf.getInt("param2");
  pattern.param3 = conf.getInt("param3");

  change_palette = RandomChange(pattern.param1);
  change_brightness = RandomChange(pattern.param2, 100, 255);
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

  Wire.begin(23, 27);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  delay(10);

  digitalWrite(SHT_LOX1, HIGH);
  delay(10);

  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println("Could not detect VL53L0X unit 1");
    while (1)
      ;
  }

  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println("Could not detect VL53L0X unit 2");
    while (1)
      ;
  }

  lox1.startRangeContinuous();
  lox2.startRangeContinuous();
}

void drawSegment(uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  CRGBPalette16 p = CRGB(pattern.base_color);
  uint16_t i = start;
  while (i != end) {
    // triwave8 creates the variations, wave_freq scales frequency (same scaling as how sine works)
    // delta shifts the pattern
    // minus start so that the patterns on different segments start at consistent points
    uint8_t pos = i + delta - start;

    if (invert) {
      pos = i - delta - start;
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
  static uint8_t delta = 0;
  static uint8_t line_length = 0;

  static uint16_t star_index = -1;

  VL53L0X_RangingMeasurementData_t measurement;
  if (lox1.getSingleRangingMeasurement(&measurement, false) == VL53L0X_ERROR_NONE) {
    Serial.println(measurement.RangeMilliMeter);
    static bool pWithinRange;

    bool withinRange = measurement.RangeMilliMeter < 1000;

    if (withinRange && !pWithinRange) {
      star_index = 0;
    }

    pWithinRange = withinRange;
  } else {
    Serial.println("Failed to range");
  }

  uint32_t start = micros();
  FastLED.setBrightness(pattern.brightness);

  if (pattern.num == '0') {
    drawSegment(0, NUM_MIDDLE, delta, false);
    drawSegment(NUM_MIDDLE, 720, delta, true);
    drawSegment(720, 720 + 35, delta, false);
    drawSegment(720 + 35, 720 + 70, delta, true);

    CRGB star_color = CRGB(pattern.sec_color);
    static uint32_t last_change = 0;

    if (millis() - last_change > pattern.param2) {
      last_change = millis();
      if (star_index < NUM_LEDS) {
        for (uint16_t i = star_index; i < (star_index + pattern.param3); i++) {
          if (i < (NUM_LEDS - 1)) {
            overlay_leds[i] = star_color;
          }
        }
        star_index += pattern.param1;
      }
    }

    fadeToBlackBy(overlay_leds, NUM_LEDS, 32);

    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      leds[i] += overlay_leds[i];
    }
  } else if (pattern.num == '1') {
    static bool color = false;
    static uint8_t last_change = 0;

    for (uint16_t i = NUM_LEDS - 1; i >= 1; i--) {
      leds[i] = leds[i - 1];
    }
    leds[0] = leds[1];

    CRGB base_color = CRGB::Black;
    CRGB whiter = base_color + CRGB(255, 255, 255);
    if (color) {
      nblend(leds[0], base_color, pattern.param1);
    } else {
      nblend(leds[0], whiter, pattern.param1);
    }

    if (last_change == 0) {
      color = !color;
      last_change = pattern.param3;
    } else if (last_change > 0) {
      last_change--;
    }

    // fill_palette(leds, NUM_LEDS, delta, 2, p, 255, LINEARBLEND);
  }

  EVERY_N_SECONDS(10) {
    Serial.print("Calculation time: ");
    Serial.print(micros() - start);
    Serial.println("us");
  }

  FastLED.show();
  FastLED.delay(1000 / FPS);

  EVERY_N_MILLISECONDS(1000 / CALC_FPS) {
    static uint8_t delta_sin = 0;

    if (delta_sin < 180) {
      delta += 1;
    } else if (delta_sin < 220) {
      delta += 2;
    } else {
      delta += 4;
    }

    delta_sin += pattern.move_speed;
  }
}

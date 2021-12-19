#include <Arduino.h>
#include <AsyncWebConfig.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 25
#define NUM_LEDS (144 * 5)
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

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
    "'name':'param1',"
    "'label':'Parameter1',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':255,"
    "'default':'60'"
    "},"
    "{"
    "'name':'param2',"
    "'label':'Parameter2',"
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
  uint16_t fps;
  uint8_t brightness;
  uint32_t base_color;
  uint16_t move_speed;
  float wave_freq;
  int8_t wave_duty;
  uint16_t param1;
  uint16_t param2;
} pattern_config_t;

pattern_config_t pattern;

void readParams() {
  pattern.fps = conf.getInt("fps");
  pattern.brightness = conf.getInt("brightness");
  pattern.base_color = strtol(conf.getString("color").c_str() + 1, NULL, 16);
  pattern.move_speed = conf.getInt("move_speed");
  pattern.wave_freq = conf.getFloat("wave_freq");
  pattern.wave_duty = conf.getInt("wave_duty");

  pattern.param1 = conf.getInt("param1");
  pattern.param2 = conf.getInt("param2");
}

void handleRoot(AsyncWebServerRequest *request) {
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

CRGB leds[NUM_LEDS];

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
}

void loop() {
  static uint8_t delta = 0;

  uint32_t start = micros();
  FastLED.setBrightness(pattern.brightness);

  CRGBPalette16 p = CRGB(pattern.base_color);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    // triwave8 creates the variations, wave_freq scales frequency (same scaling as how sine works)
    // delta shifts the pattern
    uint8_t base = triwave8(pattern.wave_freq * (i + delta));
    // Shift up/down based on wave_duty. Rescale it to max 255
    int16_t scaled = (base + pattern.wave_duty) * 255 / (255 + pattern.wave_duty);

    uint8_t bri = 0;
    if (scaled >= 0 && scaled <= 255) {
      bri = scaled;
    } else if (scaled > 255) {
      bri = 255;
    }

    leds[i] = ColorFromPalette(p, i, bri, LINEARBLEND);
  }

  EVERY_N_SECONDS(10) {
    Serial.print("Calculation time: ");
    Serial.print(micros() - start);
    Serial.println("us");
  }

  FastLED.show();
  FastLED.delay(1000 / FPS);

  EVERY_N_MILLISECONDS(1000 / CALC_FPS) {
    delta += pattern.move_speed;
  }
}

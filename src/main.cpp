#include <Arduino.h>
#include <AsyncWebConfig.h>
#include <ESPAsyncWebServer.h>

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
    "}"
    "]";

AsyncWebServer server(80);
AsyncWebConfig conf;

typedef struct params {
  uint16_t fps;
  uint32_t base_color;
} pattern_config_t;

pattern_config_t pattern;

void readParams() {
  pattern.fps = conf.getInt("fps");
  pattern.base_color = strtol(conf.getString("color").c_str() + 2, NULL, 16);

  Serial.println(pattern.fps);
  Serial.println(pattern.base_color);
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
}

void loop() {
  // put your main code here, to run repeatedly:
}

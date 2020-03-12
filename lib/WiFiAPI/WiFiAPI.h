#include <WiFi.h>
#include <WiFiAP.h>
#include <M5Stack.h>
#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "AsyncJson.h"

#include "SPIFFS.h"
#include <EEPROM.h>
#include <ezTime.h>

#include "Config.h"

#define RTE_PARAMS AsyncWebServerRequest *request
#define JSON_ROUTE(url) new AsyncCallbackJsonWebHandler(url, [](AsyncWebServerRequest *request, JsonVariant &json)
#define WIFI_STA_MAX_RETRIES 3

const char RES_OK[] PROGMEM = "{\"result\": \"ok\"}";
const char RES_JSON[] PROGMEM = "application/json";

void route_main(RTE_PARAMS);
void route_wifi_scan(RTE_PARAMS);
void route_wifi_sta(RTE_PARAMS);
void route_conf_id(RTE_PARAMS);
void route_time_ntp(RTE_PARAMS);
void route_time_synclocal(RTE_PARAMS);
void route_time_syncntp(RTE_PARAMS);

class WiFiAPI {
    public:
    WiFiAPI(M5Stack* m5);
    void begin();
    void update();
    
    /* Controling WiFi power settings*/
    void wifi_sta_connect(bool initial = true);
    void wifi_sta_disconnect();

    private:
    void setup_routes();
    void route_static(AsyncWebServerRequest *request);
    
    bool _sta_connected;
    int retries = WIFI_STA_MAX_RETRIES;

    AsyncWebServer server;
    M5Stack* board;
};

extern WiFiAPI wifi;

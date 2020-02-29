#include <WiFi.h>
#include <WiFiAP.h>
#include <M5Stack.h>
#include <WebServer.h>
#include "SPIFFS.h"

#include "Config.h"

class WiFiAPI {
    public:
    WiFiAPI(M5Stack* m5);
    void begin();
    void update();

    private:
    void setup_routes();
    void route_static();
    void route_main();
    void route_wifi_scan();
    void route_wifi_sta();
    void route_conf_name();
    

    WebServer server;
    M5Stack* board;
};

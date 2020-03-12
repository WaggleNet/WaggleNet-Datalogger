#include "WiFiAPI.h"
#include "Timekeeper.h"

using namespace std::placeholders;

#define ROUTE(RFUNC) std::bind(&WiFiAPI::RFUNC, this, _1)

WiFiAPI::WiFiAPI(M5Stack* m5)
    :server(80) {
    board = m5;
}

String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void WiFiAPI::route_static(AsyncWebServerRequest *request) {
    String path = request->url();
    Serial.print("Serving static path: ");
    Serial.println(path);
	if (path.endsWith("/")) path += "index.html";
	String contentType = getContentType(path);
	if (SPIFFS.exists(path)) {
		auto *res = request->beginResponse(SPIFFS, path);
        request->send(res);
		return;
	} else {
        request->send(404, "text/plain", "404: Not Found");
    }
}

void route_main(RTE_PARAMS) {

}

void route_wifi_sta(RTE_PARAMS) {
    auto *response = request->beginResponseStream(RES_JSON);
    StaticJsonDocument<128> result;
    result["on"] = config::is_wifi_sta_on();
    result["ssid"] = config::get(CONF_WIFI_STA_SSID);
    result["pwd"] = config::get(CONF_WIFI_STA_PWD);
    serializeJson(result, *response);
    request->send(response);
}

auto* route_wifi_sta_post = JSON_ROUTE("/api/wifi/sta") {
    JsonObject result = json.as<JsonObject>();
    config::set_wifi_sta_on(result["on"]);
    config::set(CONF_WIFI_STA_SSID, result["ssid"]);
    config::set(CONF_WIFI_STA_PWD, result["pwd"]);
    // Always disconnect WiFi first
    wifi.wifi_sta_disconnect();
    if (result["on"]) wifi.wifi_sta_connect();
    request->send(200, RES_JSON, RES_OK);
});

void route_wifi_scan(RTE_PARAMS) {
    
}

void route_conf_id(RTE_PARAMS) {
    auto *response = request->beginResponseStream(RES_JSON);
    StaticJsonDocument<128> result;
    result["id"] = config::get(CONF_DEVICE_ID);
    serializeJson(result, *response);
    request->send(response);
}

auto* route_conf_id_post = JSON_ROUTE("/api/conf/id") {
    JsonObject result = json.as<JsonObject>();
    config::set(CONF_DEVICE_ID, result["id"]);
    request->send(200, RES_JSON, RES_OK);
});

void route_time_ntp(RTE_PARAMS) {
    auto *response = request->beginResponseStream(RES_JSON);
    StaticJsonDocument<128> result;
    result["on"] = true;
    result["server"] = config::get(CONF_NTP_SERVER);
    serializeJson(result, *response);
    request->send(response);
}

void route_time_now(RTE_PARAMS) {
    auto *response = request->beginResponseStream(RES_JSON);
    StaticJsonDocument<128> result;
    result["now"] = (uint32_t)now();
    serializeJson(result, *response);
    request->send(response);
}

auto* route_time_ntp_post = JSON_ROUTE("/api/time/ntp") {
    JsonObject result = json.as<JsonObject>();
    config::set(CONF_NTP_SERVER, result["server"]);
    timekeeper.updateNTPSettings();
    request->send(200, RES_JSON, RES_OK);
});

void route_time_syncntp(RTE_PARAMS) {
    request->send(200, RES_JSON, RES_OK);
}

auto* route_time_synclocal_post = JSON_ROUTE("/api/time/sync") {
    JsonObject result = json.as<JsonObject>();
    time_t now = result["now"];
    setTime(now);
    request->send(200, RES_JSON, RES_OK);
});

void WiFiAPI::wifi_sta_connect(bool initial) {
    Serial.println(F("[wifi] Attempting connection"));
    String ssid = config::get(CONF_WIFI_STA_SSID);
    String pwd = config::get(CONF_WIFI_STA_PWD);
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
    _sta_connected = true;
    if (initial) retries = WIFI_STA_MAX_RETRIES;
}

void WiFiAPI::wifi_sta_disconnect() {
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_AP);
    _sta_connected = false;
}

void WiFiAPI::setup_routes() {
    server.on("/api/wifi/scan", HTTP_GET, route_wifi_scan);
    server.on("/api/wifi/sta", HTTP_GET, route_wifi_sta);
    server.on("/api/conf/id", HTTP_GET, route_conf_id);
    server.on("/api/time/ntp", HTTP_GET, route_time_ntp);
    server.on("/api/time/now", HTTP_GET, route_time_now);
    server.on("/api/time/ntpsync", HTTP_GET, route_time_syncntp);
    server.on("/", route_main);
    server.addHandler(route_wifi_sta_post);
    server.addHandler(route_time_synclocal_post);
    server.onNotFound(ROUTE(route_static));
    server.begin();
}


void WiFiAPI::begin() {
    String ssid = "WNLogger_";
    WiFi.mode(WIFI_MODE_AP);
    ssid += config::get(CONF_DEVICE_ID);
    WiFi.softAP(ssid.c_str());

    setup_routes();
    Serial.println("Routes setup complete!");
}

void WiFiAPI::update() {
    /* Check and maintain wifi connection */
    switch (WiFi.status()) {
        case WL_CONNECT_FAILED:
        case WL_CONNECTION_LOST:
        case WL_NO_SSID_AVAIL:
        // At connection failure, retry
        // If WiFi connection fails, decrement retries
        if (_sta_connected) {
            retries --;
            Serial.print(F("[wifi] STA Failed connection - retries left: "));
            Serial.println(retries);
            wifi_sta_connect(false);
        }
        break;

        case WL_CONNECTED:
        // If WiFi connection succeeds, reset retries
        retries = WIFI_STA_MAX_RETRIES;
        break;
    }
    // At exhaustion of retries, die
    if (!retries) {
        config::set_wifi_sta_on(false);
        wifi_sta_disconnect();
    }
}
 
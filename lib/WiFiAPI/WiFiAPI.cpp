#include "WiFiAPI.h"

using namespace std::placeholders;

#define ROUTE(RFUNC) std::bind(&WiFiAPI::RFUNC, this)

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

void WiFiAPI::route_static() {
    String path = server.uri();
    Serial.print("Serving static path: ");
    Serial.println(path);
	if (path.endsWith("/")) path += "index.html";
	String contentType = getContentType(path);
	if (SPIFFS.exists(path)) {
		auto f = SPIFFS.open(path, "r");
		server.streamFile(f, contentType);
		f.close();
		return;
	} else {
        server.send(404, "text/plain", "404: Not Found");
    }
}

void WiFiAPI::route_wifi_scan() {
    // TODO
    server.send(200, "text/plain", "todo");
}

void WiFiAPI::route_wifi_sta() {
    // TODO
    server.send(200, "text/plain", "todo");
}

void WiFiAPI::route_conf_name() {
    // TODO
    server.send(200, "text/plain", "todo");
}

void WiFiAPI::route_main() {
    // TODO
    server.send(200, "text/plain", "todo");
}

void WiFiAPI::setup_routes() {
    server.on("/api/wifi/scan", ROUTE(route_wifi_scan));
    server.on("/api/wifi/sta", ROUTE(route_wifi_sta));
    server.on("/api/conf/name", ROUTE(route_conf_name));
    server.on("/", ROUTE(route_main));
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
    server.handleClient();
}

#pragma once
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

/* Config: Reads and writes config strings in flash */

#define CONF_VERSION            "/conf/version"         // Schema version
#define CONF_DEVICE_ID          "/conf/devid"

#define CONF_WIFI_STA_ON        "/conf/wifi/on"
#define CONF_WIFI_STA_SSID      "/conf/wifi/ssid"
#define CONF_WIFI_STA_PWD       "/conf/wifi/pwd"

#define CONF_NTP_SERVER         "/conf/time/ntpserver"

#define DEFAULT_VERSION         "1.0.1"
#define DEFAULT_WIFI_STA_ON     "false"
#define DEFAULT_NTP_SERVER      "pool.ntp.org"

namespace config {
    String get(String filename);
    void set(String filename, String content);
    bool begin();
    bool is_wifi_sta_on();
    void set_wifi_sta_on(bool sta_on);
}

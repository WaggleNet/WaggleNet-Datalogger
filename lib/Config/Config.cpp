#include "Config.h"

namespace config {
    String get(String filename) {
        if (SPIFFS.exists(filename)) {
            auto f = SPIFFS.open(filename, "r");
            auto s = f.readString();
            f.close();
            return s;
        } else return String("");
    }

    void set(String filename, String content) {
        auto f = SPIFFS.open(filename, "w");
        f.print(content);
        f.close();
    }

    bool begin() {
        // Initialize the filesystem
        if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
            Serial.println("SPIFFS initialization failed");
            return false;
        }
        // Initialize the parameters by schema
        String cur_version = get(CONF_VERSION);
        if (cur_version != DEFAULT_VERSION) {
            char mac[16];
            uint64_t chipid = ESP.getEfuseMac();
            sprintf(mac, "%04X%08X", (uint16_t)(chipid>>32), (uint32_t)chipid);
            set(CONF_DEVICE_ID, String(mac));
            set(CONF_VERSION, DEFAULT_VERSION);
            set(CONF_WIFI_STA_ON, DEFAULT_WIFI_STA_ON);
            set(CONF_NTP_SERVER, DEFAULT_NTP_SERVER);
        }
    }

    // Convenience functions involving type conversions
    bool is_wifi_sta_on() {
        String data = get(CONF_WIFI_STA_ON);
        return String("true") == data;
    }

    void set_wifi_sta_on(bool sta_on) {
        String data = sta_on ? "true" : "false";
        set(CONF_WIFI_STA_ON, data);
    }
}

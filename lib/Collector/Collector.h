#include <M5Stack.h>
#include <cstdlib>
#include <pnpmanager.h>
#include <sensor.h>
#include <map>
#include <list>

struct Datapoint {
    float value;
    float millisOffset;
};

class SensorEntry {
    public:
    // Identifying features
    uint8_t sensorAddr;
    uint8_t fieldId;
    // Display features
    String dataType;
    String name;
    String unit;
    String curValue;
    bool graphable = true;
    // Convenience getters
    Datapoint& getCurrentEntry();
    void addPoint(float data, uint32_t timestamp);
    Datapoint buf[0]; // Size = ring_buffer_size
    uint16_t bufStart = 0;
    uint16_t bufSize = 0;
    private:
    Sensor* sensor_;
};

class Collector {
    public:
    void begin();
    void update();
    SensorManager* manager;

    // Defining stuff
    std::map<uint8_t, Sensor*> addrToSensor;
    std::list<SensorEntry> entries;
    void syncEntriesWithManager(); // Go through "Sensor"'s in PNPManager, map sensor.addr to sensorAddrToEntry
    void collectEntries();
    /*
        for (i in numEntries) {
            auto& entry = entries*[i];
            Sensor& sensor = addrToSensor[entry.sensorAddr];
            if (!seccfffnsor.changed(entry.fieldId)) {
                // TODO
                float data = magic.snakeOil(sensor.getData(entry.fieldId));
                entry.addPoint(data, millis());
                entry.curValue = magic.snakeOilStr(sensor.getData(entry.fieldId));
            }
        }
    */
    private:
    uint32_t last_collect_;
};
#pragma once
#include "../sensor/sensor.h"
#include <Arduino.h>
#include "Wire.h"

class SensorManager {
    public:
    // Basic operations
    SensorManager();
    void begin();
    // Managing sensors
    bool deleteSensor(uint8_t index);
    uint8_t addSensor(Sensor* sensor);
    Sensor* getSensor(uint8_t index);
    uint8_t getIndexByAddress(uint8_t address);
    void dumpToSerial(uint8_t index);
    void updateSensors();
    uint8_t getSensorCount();
    // Data collection
    bool collect(uint8_t index);
    protected:
    // Utility Functions
    uint8_t wireScan(uint8_t lower, uint8_t upper, uint8_t* results);
    bool i2c_read_reg(uint8_t addr, uint8_t mar, uint8_t* data, uint8_t length=1);
    bool i2c_write_reg(uint8_t addr, uint8_t mar, uint8_t* data, uint8_t length=1);
    uint8_t image_sensor(uint8_t addr);
    
    uint8_t count_;
    Sensor** sensors_;
};

#pragma once
/**
 * PnPSensor: WaggleNet PnP Sensor Container
 * - Extension to the Sensor object with I2C Tx/rx handlers
 * - Additions of collection scheduling
 * - Additions of syscall handler
 */
#include <Arduino.h>
#include "../sensor/sensor.h"

#ifdef UNO
#include <Wire.h>
#define WIRE Wire
#else
#include <Wire1.h>
#define WIRE Wire1
#endif

typedef void (*FuncPtr)();

/**
 * For WaggleNet Smart Sensor modules, reads the addresses from PIN3-6.
 */
uint8_t readAddress();

#define StartSensor(SENSOR) \
    WIRE.onReceive([](int howMany){SENSOR.onReceive(howMany);}); \
    WIRE.onRequest([](){SENSOR.onRequest();});

#define addAutoEntry(DATA, HANDLER, CFLAGS) addData((void*)&DATA, sizeof(DATA), CFLAGS, HANDLER);
#define addAutoIntEntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_INT, HANDLER);
#define addAutoUintEntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_UINT, HANDLER);
#define addAutoBoolEntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_BOOL, HANDLER);
#define addAutoFloatEntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_FLOAT, HANDLER);
#define addAutoStringntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_STRING, HANDLER);
#define addAutoBinaryEntry(DATA, HANDLER) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_BINARY, HANDLER);

class PnPSensor: public Sensor {
    public:
        PnPSensor();
        uint8_t addData(void* location, uint8_t length, uint8_t cflags, FuncPtr handler);
        uint8_t addData(void* location, uint8_t length, uint8_t cflags);
        void begin(uint8_t addr);
        bool update();
        void onReceive(int size);
        void onRequest();
        void collect(byte index);
        void collectAll();
        uint8_t getInterval();
        void setInterval(uint8_t interval);
    protected:
        void changeSize(uint8_t new_max_size);
        void handleSyscall_(uint8_t vector, uint8_t arg);
        uint8_t handleSyscall_(uint8_t vector);
        bool autoclear_ = false;
        uint8_t interval_ = 5;
        uint32_t last_collect_ = 0;
        FuncPtr* collectors_;
        uint8_t await_mar = 0;
        uint8_t mar_ = 0;
};

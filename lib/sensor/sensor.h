#pragma once
#ifdef __AVR_ARCH__
    // C Stdlib for AVR
    #include <stdint.h>
    #include <math.h>
#else
    // When testing this on a computer
    #include <cstdint>
    #include <cmath>

    using namespace std;
#endif
#include "flags.h"

#ifndef NULL
// REALLY you wanna do this to me?!
#define NULL 0x0
#endif

#define VERSION 1
#define ADDR_BEGIN 0x20  // I2C address starts at 0x20

// Callback from change in sensor data value
// Should take a parameter for index of changed field
typedef void (*ChangeCallbackFuncPtr)(uint8_t);

class Sensor {
    public:
        // Constructor
        Sensor(uint8_t max_size);
        Sensor();
        // Manipulating data
        uint8_t addData(void* location, uint8_t length, uint8_t cflags);
        uint8_t getLength(uint8_t index);
        void* getData(uint8_t index);
        uint8_t hasChanged(uint8_t index);
        uint8_t getSize();
        uint8_t getDataType(uint8_t index);
        uint8_t getCflag(uint8_t index);
        uint8_t getVflag(uint8_t index);
        uint8_t fireCallback(uint8_t index);
        void setCallback(uint8_t index, ChangeCallbackFuncPtr fn);
        void changed(uint8_t index);
        void changed(uint8_t index, uint8_t changed);
        void clearChanged();
        #define addEntry(DATA, CFLAGS) addData((void*)&DATA, sizeof(DATA), CFLAGS)
        #define addIntEntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_INT);
        #define addUintEntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_UINT);
        #define addBoolEntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_BOOL);
        #define addFloatEntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_FLOAT);
        #define addStringntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_STRING);
        #define addBinaryEntry(DATA) addData((void*)&DATA, sizeof(DATA), DATA_TYPE_BINARY);
        // Useful attributes
        uint8_t address;
        uint8_t version;
        uint32_t type;
        // Register Translation
        uint8_t* getRegister(uint8_t mar);
        uint8_t getRegisterSize(uint8_t mar);
        bool setRegister(uint8_t mar, uint8_t* data);
    protected:
        uint8_t size_;
        uint8_t max_size_;
        void** data_;
        uint8_t* lengths_;
        uint8_t* cflags_;
        uint8_t* vflags_;
        ChangeCallbackFuncPtr* change_callbacks_;
        void changeSize(uint8_t new_max_size);
};

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

#ifndef NULL
// REALLY you wanna do this to me?!
#define NULL 0x0
#endif

#define VERSION 1

class Sensor {
    public:
        // Constructor
        Sensor(uint8_t max_size);
        Sensor();
        // Manipulating data
        uint8_t addData(void* location, uint8_t length);
        uint8_t getLength(uint8_t index);
        void* getData(uint8_t index);
        uint8_t hasChanged(uint8_t index);
        uint8_t getSize();
        void changed(uint8_t index);
        void changed(uint8_t index, uint8_t changed);
        void clearChanged();
        #define addEntry(DATA) addData((void*)&DATA, sizeof(DATA))
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
        uint8_t* changed_;
        void changeSize(uint8_t new_max_size);
};

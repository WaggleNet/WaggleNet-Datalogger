#include "pnpsensor.h"

PnPSensor::PnPSensor():Sensor() {
    
}

uint8_t PnPSensor::addData(void* location, uint8_t length, FuncPtr handler) {
    uint8_t index = PnPSensor::addData(location, length);
    collectors_[index] = handler;
    return index;
}

uint8_t PnPSensor::addData(void* location, uint8_t length) {
    // First check if the data array has ever been initialized.
    if (size_ == max_size_) changeSize(max_size_ + 1);
    // Insert data at last location
    data_[size_] = location;
    lengths_[size_] = length;
    changed_[size_] = 0;
    // Increment size
    size_ ++;
    return size_ - 1;
}

void PnPSensor::begin(uint8_t addr = 0x35) {
    // Set up I2C
    WIRE.begin(addr);
    // Initialize state machine
    await_mar = 0;
    // Initialize timer
    last_collect_ = 0;
}

bool PnPSensor::update() {
    // Just collect data
    if ((long)millis() - last_collect_ >= 1000L * interval_) {
        collectAll();
        last_collect_ = millis();
        return true;
    }
    return false;
}

void PnPSensor::onReceive(int size) {
    #ifdef DEBUG
    if (!size) {
        Serial.println("Rx -!> No data");
    }
    #endif
    // If just waiting for MAR, set buffer
    if (!await_mar) {
        mar_ = WIRE.read();
        await_mar = 1;
        #ifdef DEBUG
        Serial.print("Rx --> MAR = ");
        Serial.println((int)mar_);
        #endif
        // If there's still data, call back with MDR setting
        if (size-1) onReceive(size-1);
    } else { 
        // First check the size
        auto expected_length = getRegisterSize(mar_);
        if (size != expected_length) {
            #ifdef DEBUG
            Serial.println("Rx -!> Size mismatch");
            #endif
            while (WIRE.available()) WIRE.read();
        } else {
            if (mar_ < 192) {
                #ifdef DEBUG
                Serial.println("Rx --> Set MDR");
                #endif
                byte buffer[expected_length];
                byte counter = 0;
                while (WIRE.available()) {
                    buffer[counter] = WIRE.read();
                    counter ++;
                }
                setRegister(mar_, buffer);
            } else {
                // Handle Syscall
                #ifdef DEBUG
                Serial.println("Rx --> Syscall");
                #endif
                auto ch = WIRE.read();
                handleSyscall_(mar_, ch);
            }
        }
        // Flip the FSM back to 0
        await_mar = 0;
    }
}

void PnPSensor::onRequest () {
    #ifdef DEBUG
    Serial.println("Tx --> Req");
    #endif
    // First off, if I don't know what to send, send nothing
    if (!await_mar) return;
    // Now send the things
    auto size = getRegisterSize(mar_);
    #ifdef DEBUG
    Serial.print("Tx --> Data of size ");
    #endif
    #ifdef DEBUG
    Serial.println(size);
    #endif
    // Handle syscall
    if (mar_ >= 192) {
        WIRE.write(handleSyscall_(mar_));
    } else {
        auto data = getRegister(mar_);
        #ifdef DEBUG
        Serial.print("Tx --> Data readout: 0x");
        #endif
        for (byte i = 0; i < size; i++) {
            WIRE.write(data[i]);
            #ifdef DEBUG
            Serial.print(data[i], HEX);
            #endif
        }
        #ifdef DEBUG
        Serial.println();
        #endif
        // If there's autoclear, now clear the flag
        if (autoclear_ && (!mar_ % 4)) changed(mar_ / 4 - 1, 0);
    }
    // Set the FSM to 0
    await_mar = 0;
}

void PnPSensor::collect(byte index) {
    if (collectors_[index] != NULL) {
        #ifdef DEBUG
        Serial.print("> Collecting data from ");
        Serial.println(index, DEC);
        #endif
        collectors_[index]();
        changed(index);
    }
}

void PnPSensor::collectAll() {
    for (byte i = 0; i < size_; i++) collect(i);
}

void PnPSensor::setInterval(uint8_t interval) {
    interval_ = interval;
}

uint8_t PnPSensor::getInterval() {
    return interval_;
}

void PnPSensor::handleSyscall_(uint8_t vector, uint8_t arg) {
    switch (vector) {
        case 192:
            // 192: Force collect all data
            collectAll();
            break;
        case 193:
            // Force clear changed flags
            for (byte i = 0; i < size_; i++) changed(i, 0);
            break;
        case 194:
            // Configure Autoclear
            autoclear_ = arg;
            break;
        case 200:
            // Force restart
            asm volatile ("  jmp 0");
            break;
        default:
            return;
    }
}

uint8_t PnPSensor::handleSyscall_(uint8_t vector) {
    switch (vector) {
        case 194:
            // Return autoclear config
            return autoclear_;
        default:
            return 0;
    }
}

void PnPSensor::changeSize(uint8_t new_max_size) {
    auto new_collectors_ = new FuncPtr[new_max_size];
    for (uint8_t i = 0; i < new_max_size; i++)
        new_collectors_[i] = NULL;
    if (max_size_ > 0) {
        for (uint8_t i = 0; i < max_size_; i++) {
            new_collectors_[i] = collectors_[i];
        }
        delete[] collectors_;
    }
    collectors_ = new_collectors_;
    Sensor::changeSize(new_max_size);
}

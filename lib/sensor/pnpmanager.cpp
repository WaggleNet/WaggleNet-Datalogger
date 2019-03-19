#include "pnpmanager.h"

SensorManager::SensorManager() {

}

void SensorManager::begin() {
    Wire.begin();
    updateSensors();
}

void SensorManager::updateSensors() {
    uint8_t* scanResults;
    wireScan(0x60, 0x70, scanResults);
    // what is the point of this
    delete[] scanResults;
    Serial.println("> Adding 0x62 to sensors");
    image_sensor(0x62);
}

uint8_t SensorManager::wireScan(uint8_t lower, uint8_t upper, uint8_t*& results) {
    results = new uint8_t[32];
    uint8_t error;
    uint8_t counter = 0;
    Serial.println(F("-!>\tSTART\tSCANNING"));
    for (uint8_t addr=lower; addr < upper; addr++) {
        Wire.beginTransmission(addr);
        error = Wire.endTransmission(true);
        if (!error) {
            results[counter] = addr;
            counter ++;
            Serial.print(F("-->\tSensor.Address\t"));
            Serial.println(addr, HEX);
        }
    }
    Serial.print("-!>\tOK\tFOUND\t");
    Serial.println((int)counter);
    return counter;
}

/**
 * Scan and build an image of the sensor at addr,
 * and add to the list of sensors.
 * :return: Address of the sensor. If error occured, returns 255.
 */
uint8_t SensorManager::image_sensor(uint8_t addr) {
    // Prelim: Read version of sensor, make sure it's acceptable
    uint8_t version, length, result, size;
    String fail_msg;
    bool res;
    Sensor* sensor = NULL;
    // question : length not specified
    res = i2c_read_reg(addr, 0, &version, 1);
    // TODO: Bail
    // question : same herere
    res = i2c_read_reg(addr, 2, &length, 1);
    // TODO: Bail
    Serial.print(F("-->\tSensor.Version\t"));
    Serial.println(version, HEX);
    Serial.print(F("-->\tSensor.Size\t"));
    Serial.println(length, HEX);
    if (!version || !length) {
        fail_msg = "Invalid sensor profile";
        goto bail;
    } 
    // Now construct the object and ask for more profile-type questions
    sensor = new Sensor(length);
    sensor->address = addr;
    sensor->version = version;
    res = i2c_read_reg(addr, 1, (uint8_t*)&(sensor->type), 4);
    // TODO: Warn
    return 0;
    for (uint8_t i = 0; i < length; i++) {
        res = i2c_read_reg(addr, 4+4*i+2, &size);
        if (!res){
            fail_msg = "Error when reading register ";
            fail_msg += (int)i;
            goto bail;
        }
        sensor->addData(new uint8_t[size], size);
        Serial.print(F("-->\tEntry.Index\t"));
        Serial.println(i, DEC);
        Serial.print(F("-->\tEntry.Length\t"));
        Serial.println(size, DEC);
    }
    // Success
    result = addSensor(sensor);
    Serial.print(F("-->\tSensor.Index\t"));
    Serial.println(result);
    Serial.println("-!>\tOK");
    return result;
    bail:
        Serial.print("-!>\tERR\t");
        Serial.println(fail_msg);
        if (sensor == NULL) delete sensor;
        return 255;
}

uint8_t SensorManager::addSensor(Sensor* sensor) {
    Sensor** new_sensors_ = new Sensor*[count_+1];
    // Copy over the old data + new sensor
    for (uint8_t i = 0; i < count_; i++) new_sensors_[i] = sensors_[i];
    new_sensors_[count_] = sensor;
    // Delete and replace the array
    delete[] sensors_;
    sensors_ = new_sensors_;
    count_ ++;
    return count_ - 1;
}

bool SensorManager::deleteSensor(uint8_t index) {
    // If no such index, go home
    if (index >= count_) return false;
    // Copy over the indices
    Sensor** new_sensors_ = new Sensor*[count_-1];
    for (uint8_t i = 0; i < index; i++) new_sensors_[i] = sensors_[i];
    for (uint8_t i = index+1; i < count_; i++) new_sensors_[i-1] = sensors_[i];
    // Replace the array
    delete[] sensors_;
    sensors_ = new_sensors_;
    count_ --;
    return true;
}

bool SensorManager::i2c_read_reg(uint8_t addr, uint8_t mar, uint8_t* data, uint8_t length) {
    Wire.beginTransmission(addr);
    Wire.write(mar);
    Serial.println("transmission");
    Wire.endTransmission(false);
    Serial.println("end transmission");
    auto l = Wire.requestFrom(addr, length);
    delay(10);
    Serial.println(l);
    if (l != length) return false;
    Serial.print("--> Data readout: 0x");
    for (uint8_t i = 0; i < length; i++) {
        data[i] = Wire.read();
        Serial.print(data[i], HEX);
    }
    Serial.println();
    return true;
}

bool SensorManager::i2c_write_reg(uint8_t addr, uint8_t mar, uint8_t* data, uint8_t length) {
    Wire.beginTransmission(addr);
    Wire.write(mar);
    Wire.endTransmission(false);
    Wire.beginTransmission(addr);
    auto res = Wire.write(data, length);
    return res;
}

void SensorManager::dumpToSerial(uint8_t index) {
    auto& s = *(sensors_[index]);
    Serial.println(F("-!>\tSTART\tDump.Sensor"));
    Serial.print(F("-->\tSensor.Address\t0x"));
    Serial.println(s.address, HEX);
    Serial.print(F("-->\tSensor.Type\t0x"));
    Serial.println(s.type, HEX);
    Serial.print(F("-->\tSensor.Size\t"));
    Serial.println(s.getSize(), DEC);
    Serial.println(F("-!>\tSTART\tDump.Data"));
    for (byte i = 0; i < s.getSize(); i++) {
        auto data = (uint8_t*)s.getData(i);
        auto len = s.getLength(i);
        Serial.print(F("-->\tEntry.Index\t"));
        Serial.println(i, DEC);
        Serial.print(F("-->\tEntry.Length\t"));
        Serial.println(len, DEC);
        Serial.print(F("-->\tEntry.Data\t"));
        for (byte j = 0; j < len; j++) {
            Serial.print(data[j], HEX);
            Serial.print(' ');
        }
        Serial.println();
    }
    Serial.println(F("-!>\tOK"));
}

bool SensorManager::collect(uint8_t index) {
    uint8_t changed;
    Serial.print(F("-!>\tSTART\tCollect.Sensor\t"));
    Serial.println(index, DEC);
    auto& s = *(sensors_[index]);
    auto addr = s.address;
    for (byte i = 0; i < s.getSize(); i++) {
        // First determine if the sensor is changed
        i2c_read_reg(addr, 4+4*i+1, &changed, 1);
        if (changed) {
            Serial.print(F("-->\tEntry.Changed\t"));
            Serial.println(i);
            auto len = s.getLength(i);
            auto data = (uint8_t*) s.getData(i);
            i2c_read_reg(addr, 4+4*i, data, len);
            // Set local copy as changed as well
            s.changed(i);
        }
        // Serial.print("--> Setting data entry ");
        // Serial.println(i, DEC);
        // Clear all changed flags via SysCall 193
        i2c_write_reg(addr, 193, &changed, 1);
    }
    return true;
}

Sensor* SensorManager::getSensor(uint8_t index) {
    if (index >= count_) return nullptr;
    return sensors_[index];
}

uint8_t SensorManager::getIndexByAddress(uint8_t address) {
    for (uint8_t i = 0; i < count_; i++) {
        if (sensors_[i]->address == address) return i;
    }
    return count_;
}
#include "./sensor.h"

Sensor::Sensor (): Sensor(0) {}

Sensor::Sensor (uint8_t max_size):size_(0), max_size_(0) {
    if (max_size == 0) {
        data_ = NULL;
        lengths_ = NULL;
        vflags_ = NULL;
        cflags_ = NULL;
    } else {
        changeSize(max_size_);
    }
    version = VERSION;
}

void Sensor::changeSize (uint8_t new_max_size) {
    // First just add new arrays
    void** new_data_ = new void*[new_max_size];
    uint8_t* new_lengths_ = new uint8_t[new_max_size];
    uint8_t* new_vflags_ = new uint8_t[new_max_size];
    uint8_t* new_cflags_ = new uint8_t[new_max_size];
    ChangeCallbackFuncPtr* new_callbacks_ = new ChangeCallbackFuncPtr[new_max_size];
    for (uint8_t i = 0; i < new_max_size; i++) {
        new_data_[i] = NULL;
        new_lengths_[i] = 0;
        new_vflags_[i] = 0;
        new_cflags_[i] = 0;
        new_callbacks_[i] = 0;
    }
    // If there was old data, migrate the data
    if (max_size_ > 0) {
        for (uint8_t i = 0; i < (max_size_ < new_max_size ? max_size_ : new_max_size); i++) {
            new_data_[i] = data_[i];
            new_lengths_[i] = lengths_[i];
            new_vflags_[i] = vflags_[i];
            new_cflags_[i] = cflags_[i];
            new_callbacks_[i] = change_callbacks_[i];
        }
        delete[] data_;
        delete[] lengths_;
        delete[] vflags_;
        delete[] cflags_;
        delete[] change_callbacks_;
    }
    // Commit transition
    max_size_ = new_max_size;
    data_ = new_data_;
    lengths_ = new_lengths_;
    vflags_ = new_vflags_;
    cflags_ = new_cflags_;
    change_callbacks_ = new_callbacks_;
}

/**
 * Adds data to the internal data list.
 * @param location: pointer to the variable casted as void*
 * @param length: size of the variable.
 * @return: Index to the data.
 */
uint8_t Sensor::addData (void* location, uint8_t length, uint8_t cflags) {
    // First check if the data array has ever been initialized.
    if (size_ == max_size_) changeSize(max_size_ + 1);
    // Insert data at last location
    data_[size_] = location;
    lengths_[size_] = length;
    vflags_[size_] = 0;
    cflags_[size_] = cflags;
    // Incement size
    size_ ++;
    return size_ - 1;
}

/**
 * Retreives the length of the data in the indexed slot
 * @param index: Index to the data
 */
uint8_t Sensor::getLength (uint8_t index) {
    if (index > size_) return 0;
    return lengths_[index];
}

/**
 * Retrieves the pointer to the data in the given slot
 * @param index: Index to the data
 */
void* Sensor::getData (uint8_t index) {
    if (index > size_) return NULL;
    return (void*) data_[index];
}

/**
 * Run callback function if any exists.
 * return: whether a callback function exists and is run
 */
uint8_t Sensor::fireCallback(uint8_t index) {
    if (index > size_) return 0;
    if (!change_callbacks_[index]) return 0;
    // Now run the func
    change_callbacks_[index](index);
    return 1;
}

void Sensor::setCallback(uint8_t index, ChangeCallbackFuncPtr fn) {
    if (index > size_) return;
    change_callbacks_[index] = fn;
}

/**
 * Retrieves whether the data has changed over the last fetch
 */

uint8_t Sensor::hasChanged (uint8_t index) {
    if (index > size_) return 0;
    return vflags_[index] & (1 << VFLAG_CHANGED_LOC);
}

/**
 * Set whether the data has changed / not changed
 */
void Sensor::changed(uint8_t index, uint8_t changed) {
    if (!changed) {
        vflags_[index] &= ~(1 << VFLAG_CHANGED_LOC);
    } else {
        vflags_[index] |= 1 << VFLAG_CHANGED_LOC;
        fireCallback(index);
    }
}

/**
 * Mark the given entry as changed
 */
void Sensor::changed(uint8_t index) {
    changed(index, true);
}

/**
 * Get the total number of data entries in the system
 */
uint8_t Sensor::getSize () {
    return size_;
}

uint8_t Sensor::getCflag(uint8_t index) {
    if (index > size_) return 0;
    return cflags_[index];
}

uint8_t Sensor::getVflag(uint8_t index) {
    if (index > size_) return 0;
    return vflags_[index];
}

uint8_t Sensor::getDataType(uint8_t index) {
    if (index > size_) return 0;
    return getCflag(index) & CFLAG_TYPE_MASK;
}

/**
 * Translating from register number to data pointer
 * ====REGISTER MAP====
 * 0: Sensor protocol version
 * 1: Sensor Type
 * 2: Data Size
 * 3: (Reserved)
 * 4+4*k: Data in the k_th entry
 * 4+4*k+1: Variable Flags
 * 4+4*k+2: Length of the k_th entry
 * 4+4*k+3: Constant Flags
 * 200~255: Reserved Commands
 */
uint8_t* Sensor::getRegister(uint8_t mar) {
    if (mar == 0)
        return &version;
    else if (mar == 1)
        return (uint8_t*) &type;
    else if (mar == 2)
        return &size_;
    else if (mar>=4) {
        uint8_t k = (mar - 4) / 4;
        if (mar % 4 == 1)
            // Asking for variable flags
            return &vflags_[k];
        else if (mar % 4 == 2)
            // Asking for data length
            return &lengths_[k];
        else if (mar % 4 == 3)
            // Asking for constant flags
            return &lengths_[k];
        else return (uint8_t*) data_[k];
    }
    // Undefined behavior for undefined registers
    return &address;
}

uint8_t Sensor::getRegisterSize(uint8_t mar) {
    if (mar >= 192) return 1; // syscall variable
    if (mar == 1)
        return sizeof(type);
    else if (mar == 2)
        return 1;
    else if (mar>=4) {
        uint8_t k = (mar - 4) / 4;
        if (mar % 4 >= 1) // Requesting for size / vflag / cflag
            return 1;
        else return lengths_[k];
    }
    return 1;
}

bool Sensor::setRegister(uint8_t mar, uint8_t* data) {
    // First off, if the MAR is not writable at all, ignore it
    if (mar % 4 > 1) return false;  // Length, cflags
    if (mar < 4) return false;  // Metadata
    // Now find out how long the data is
    uint8_t idx = mar / 4 - 1;
    uint8_t len = lengths_[idx];
    for (uint8_t i = 0; i < len; i++) {
        // Copy the data in
        ((uint8_t*)data_[idx])[i] = data[i];
    }
    // If setting data, implicitly set the changed flag
    if (mar % 4 == 0) {
        vflags_[idx] |= 1 << VFLAG_CHANGED_LOC;
        // Also fire the changed callback
        fireCallback(idx);
    }
    return true;
}

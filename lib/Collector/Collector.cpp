#include "Collector.h"

//Collector is supposed to identify sensors, keep track of them, and their data.

void Collector::begin(){
//Starts the sensor manager, runs updateSensors();
 manager->begin(); 
/*
    Seems like updateSensors goes from 0x20 to 0x30 scanning 
    for sensors at those mem addresses. 
    num_found is the number of sensors that were found.

    wireScan works by scanning each address in the given range
    and attempting to begin/end transmission. If there is no error,
    then it is assumed a sensor is there and its address is 
    added to our results array at the index counter.
    The counter is then incremented and this repeats.

    num_found is returned, and the addresses of our sensors
    are stored in the properly named "scanResults", which was
    used as an input to wireScan. 

    imaging_sensor seems to check if two addresses in the MAR
    are readable, if they aren't then the function goes to a 
    yet to be made "bail" function. 

    WHAT IS: cflags, size in imaging_sensor. They aren't defined
    when they are instantiated at the beginning of the func.

    If all goes well the sensor is added to an array of sensors
    "sensors_" (double pointer?) and the index of the sensor in 
    said array is returned (for the purpose of printing to serial).

    After all that is done updateSensors is finished.
    So there should be a manager->sensors_ array that holds all the
    info about the sensors.
*/
}

void Collector::update(){
    manager->updateSensors();
}

void Collector::syncEntriesWithManager(){
    //manager->getSensor(uint8_t index) exists
    //as does manager->getSensorCount();   

    //TODO: more efficient way to sync?
    for(int i; i < manager->getSensorCount();i++){
        Sensor* currentSens = manager->getSensor(i);
        uint8_t sensorAddr = currentSens->address;
        addrToSensor.insert(pair<uint8_t, Sensor*>(sensorAddr, currentSens));
    }
    //should work the first time, may not work on second update.
    //as changes to sensors isn't accounted for
    //maybe just clear the map before syncing.

}
void Collector::collectEntries(){
    //I'm pretty sure this is collecting data. 
    //TODO: look through pnpsensor and see what's going on in there
    //then make this
}
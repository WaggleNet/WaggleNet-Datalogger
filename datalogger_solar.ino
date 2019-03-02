#include <Arduino.h>
#include "datalogger.h"
#include <APDS9930.h>
#include <Adafruit_INA219.h>
#include <Wire.h>

  Adafruit_INA219 ina219_A;
  Adafruit_INA219 ina219_B(0x41);
  
// Initialize all global varaibles
    float shuntvoltage_A = 0;
    float shuntvoltage_B = 0;
    float busvoltage_A = 0;
    float busvoltage_B = 0;
    float current_mA_A = 0;
    float current_mA_B = 0;
    float loadvoltage_A = 0;
    float loadvoltage_B = 0;
    float power_mW_A = 0;
    float power_mW_B = 0;
    APDS9930 apds = APDS9930();
    float ambient_light = 0;
    uint16_t ch0 = 0;
    uint16_t ch1 = 1;

void collect() {
    // Need a comma before each entry

    // Getting the inputs for each data collected from sensor A and sensor B
    shuntvoltage_A = ina219_A.getShuntVoltage_mV();
    shuntvoltage_B = ina219_B.getShuntVoltage_mV();
    busvoltage_A = ina219_A.getBusVoltage_V();
    busvoltage_B = ina219_B.getBusVoltage_V();
    current_mA_A = ina219_A.getCurrent_mA();
    current_mA_B = ina219_B.getCurrent_mA();
    power_mW_A = ina219_A.getPower_mW();
    power_mW_B = ina219_B.getPower_mW();
    loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
    loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000);
    apds.readAmbientLightLux(ambient_light);
    apds.readCh0Light(ch0);
    apds.readCh1Light(ch1);


    /* Put entry here. If unsure you can always "Serial.print" to test (If you want to test
       remember to uncomment the lines).
    */
    dataFile.print(busvoltage_A); // Serial.print(busvoltage_A); Serial.println(",");
    dataFile.print(",");
    dataFile.print(busvoltage_B); // Serial.print(busvoltage_B); Serial.println(",");
    dataFile.print(",");
    dataFile.print(shuntvoltage_A); // Serial.print(shuntvoltage_A); Serial.println(",");
    dataFile.print(",");
    dataFile.print(shuntvoltage_B); // Serial.print(shuntvoltage_B); Serial.println(",");
    dataFile.print(",");
    dataFile.print(loadvoltage_A); // Serial.print(loadvoltage_A); Serial.println(",");
    dataFile.print(",");
    dataFile.print(loadvoltage_B); // Serial.print(loadvoltage_B); Serial.println(",")
    dataFile.print(",");
    dataFile.print(current_mA_A); // Serial.print(current_mA_A); Serial.println(",");
    dataFile.print(",");
    dataFile.print(current_mA_B); // Serial.print(current_mA_B); Serial.println(",");
    dataFile.print(",");
    dataFile.print(power_mW_A); // Serial.print(power_mW_A); Serial.println(",");
    dataFile.print(",");
    dataFile.print(power_mW_B); // Serial.print(power_mW_B); Serial.println(",");
    dataFile.print(",");
    dataFile.print(ambient_light); // Serial.print(ambient_light); Serial.println(",");
    dataFile.print("/n"); // Serial.print("/n");

    delay(1000);
}

void setup() {
    ina219_A.begin();
    ina219_B.begin();
     /* To use a slightly lower 32V, 1A range (higher precision on amps):
    ina219.setCalibration_32V_1A();
    Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    ina219.setCalibration_16V_400mA();
  */
    Serial.begin(115200);
    beginDatalogger();
    setCollector(collect);
    // Initialize APDS-9930
    // You can get rid of the warning line code if necessary
    if ( apds.init() ) {
    Serial.println(F("APDS-9930 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
  // Start running the APDS-9930 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  uint32_t currentFrequency;
  /* Initialize the INA219.
     By default the initialization will use the largest range (32V, 2A).  However
     you can call a setCalibration function to change this range
  */
 
    delay(2000); // Have a bit of a time delay after initialization
}

void loop() {
    loopDatalogger();



}

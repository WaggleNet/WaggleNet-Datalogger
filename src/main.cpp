#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>
//#include <Graph.h>
//#include "sensor.h"
#include <pnpmanager.h>

#include "Config.h"
#include "WiFiAPI.h"

SensorManager manager;
Grid mainGrid(&M5, &manager);
WiFiAPI wifi(&M5);
// Graph mainGraph(250, 200, &M5);
//int flag = 0;

void setup() {
  Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.clearDisplay();
  pinMode(36, INPUT); 
  config::begin();
  wifi.begin();
  
  // Start the GUI
  mainGrid.begin();

  // Speaker shutup
  M5.Speaker.end();
  dacWrite(SPEAKER_PIN, 0);
}

void loop() {
  // mainGraph.updateGraph();
  // delay(200);
  mainGrid.update();
  wifi.update();
  // manager.collect(0);
  // manager.interpretData(0);
  // delay(5000);
}

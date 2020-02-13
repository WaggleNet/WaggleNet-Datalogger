#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>
//#include <Graph.h>
//#include "sensor.h"
#include <pnpmanager.h>


Grid mainGrid(&M5);
// Graph mainGraph(250, 200, &M5);
//int flag = 0;

void setup() {
  Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.clearDisplay();
  mainGrid.begin();
  pinMode(36, INPUT); 
}

void loop() {
  // mainGraph.updateGraph();
  //delay(200);
  mainGrid.update();
  // manager.interpretData(0);
  // delay(5000);
}

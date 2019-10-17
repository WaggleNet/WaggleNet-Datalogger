#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>
//#include <Graph.h>
//#include "sensor.h"
#include "pnpmanager.h"

SensorManager manager;
Grid mainGrid(&M5);
//Graph mainGraph(250, 200, &M5);
//int flag = 0;

void setup() {
  Serial.begin(115200);
  //delay(5000);
  manager.begin();
  
  M5.begin();
  M5.Lcd.clearDisplay();
  mainGrid.begin();
  pinMode(36, INPUT);
  //mainGraph.startGraph();
}

void loop() {
  //mainGraph.updateGraph();
  //delay(200);
  mainGrid.update();

  manager.collect(0);
  manager.interpretData(0);
  delay(5000);
}
#include <Arduino.h>
#include <M5Stack.h>
#include <pnpmanager.h>
#include "UserInterface.h"
#include "GridView.h"
#include "apps.h"


UserInterface ui(&M5);
auto* gridView = new GridView(&ui);
M5Stack* board = &M5;

void setup() {
  Serial.begin(115200);

  M5.begin();
  M5.Lcd.clearDisplay();
  pinMode(36, INPUT);

  // Initialize all the views
  Serial.print((long)&M5, HEX);
  ui.addApp((View*)gridView);
  ui.begin(APP_VIEW_GRID);
}

void loop() {
  // mainGraph.updateGraph();
  //delay(200);
  ui.update();
  // manager.interpretData(0);
  // delay(5000);
}

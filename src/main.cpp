#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>

Grid mainGrid(&M5);

void setup() {
  M5.begin();
  M5.Lcd.clearDisplay();
  mainGrid.begin();
}

void loop() {
}
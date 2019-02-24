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
  M5.update();
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (M5.BtnA.wasReleased())
    mainGrid.left();
  else if (M5.BtnC.wasReleased())
    mainGrid.right();
}
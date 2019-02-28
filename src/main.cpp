#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>

Grid mainGrid(&M5);
int flag = 0;

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
  else if (M5.BtnB.pressedFor(2000) && flag == 0) {
    mainGrid.record();
    flag = 1;
  }
  else if (M5.BtnB.wasReleased() && flag == 1) {
    flag = 0;
  }
  else if (M5.BtnB.wasReleased() && flag == 0) {
    // mainGrid.select();
  }
}
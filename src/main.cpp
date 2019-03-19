#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>
#include "sensor.h"
#include "pnpmanager.h"

SensorManager manager;
Grid mainGrid(&M5);
int flag = 0;

void setup() {
  Serial.begin(9600);
  delay(5000);
  manager.begin();
  
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

/*
#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  Wire.requestFrom(0x68, 6);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}*/
#include <Arduino.h>
#include <M5Stack.h>
#include <Grid.h>

M5Stack main;
Grid mainGrid(main);
int i = 0;

void setup() {
  main.begin();
  main.Lcd.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly
  mainGrid.begin();
  main.Lcd.fillRect(300,i,10,5,RED);
  i++;
  delay(10);
}
#include "Grid.h"
#include <stdlib.h>

Grid::Grid(M5Stack* other, SensorManager* sens_mng)
{
  block = 0;
  board = other;
  pages = 6;
  pageNum = 0;
  flag = 0;
  recording = false;
  isGraph = false;

  manager = sens_mng;
}

void Grid::update() {
  board->update();
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (board->BtnA.wasReleased())
    left();
  else if (board->BtnC.wasReleased())
    right();
  else if (board->BtnB.pressedFor(2000) && flag == 0) {
    record();
    flag = 1;
  }
  else if (board->BtnB.wasReleased() && flag == 1) {
    flag = 0;
  }
  else if (board->BtnB.wasReleased() && flag == 0) {
    select();
  }

  // if (isGraph) graph->updateGraph();
  
  updateSensors();
}

void Grid::begin()
{
  // Get that sensor manager goin
  manager->begin();

  board->Lcd.clearDisplay();
  // Draw grid
  for (int i = 0; i < 6; i++) {
    draw(i);
  }

  // Draw page indicators
  for (int i = 0; i < pages; i++) {
    int centerX = 316/2;
    int centerY = 204;
    int offset;

    if(pages%2 == 1) {
      offset = 20*(i-pages/2);
    }
    else {
      offset = 20*(i-(pages-1)/2) - 10;
    }

    if (i == pageNum)
      board->Lcd.fillCircle(centerX+offset,centerY,4,WHITE);
    else {
      board->Lcd.fillCircle(centerX+offset,centerY,4,BLACK);
      board->Lcd.drawCircle(centerX+offset,centerY,4,WHITE);
    }
  }
  
  // Draw header and footer
  board->Lcd.fillRect(0,0,316,28,WHITE);
  board->Lcd.fillRect(0,211,316,30,WHITE);
  board->Lcd.setTextColor(BLACK, WHITE);
  board->Lcd.setTextFont(0);
  board->Lcd.setTextSize(2);
  board->Lcd.setCursor(5, 5);
  if (recording)
    board->Lcd.print("Currently Recording...");
  else {
    board->Lcd.print("Ready! Hold    to record.");
    board->Lcd.fillRect(155,5,16,16,BLACK);
  }

  // control markers
  int centerX = 320/2;
  int centerY = 225;
  
  // select button
  board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);
  board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);

  // L/R buttons
  board->Lcd.fillTriangle(centerX-80,centerY-8,centerX-80,centerY+8,centerX-90,centerY,BLACK);
  board->Lcd.fillTriangle(centerX+80,centerY-8,centerX+80,centerY+8,centerX+90,centerY,BLACK);
}

void Grid::updateSensors() {
  int sensor_count= manager->getSensorCount();
  // Serial.println(sensor_count, HEX);
  // Just a test statement below
  sensor_count = 1;

  // Update every sensor & output data to screen
  for(int i = 0; i < sensor_count; i++) {
    bool flag = manager->collect(i);
    Sensor* cur_sensor = manager->getSensor(i);
    drawSensorVal(i);
    cur_sensor->changed(0, false);
  }
}

void Grid::drawSensorVal(int idx) {
  Sensor* cur_sensor = manager->getSensor(idx);
  uint8_t data_idx = cur_sensor->getSize();
  // Might need to do idx - 1 

  for (int i = 0; i < data_idx; i++) {
    void* sensor_data = cur_sensor->getData(i);
    uint8_t type = cur_sensor->getDataType(i);
    float* int_data = (float*)sensor_data;
    // Serial.println(*int_data, 4);
    Serial.println(data_idx, HEX);
    Serial.println(idx, HEX);
    Serial.println(graph_pos, HEX);
    Serial.println();

    if (isGraph && graph_pos == i+idx) graph->updateGraph(*int_data);
    else if(!isGraph) drawSensorValGrid(*int_data, idx+i);
  }
}

void Grid::drawSensorValGrid(float data, int idx) {
  int16_t xPos = (320/3)*(idx%3);
  int16_t yPos = 30+(170/2)*(idx/3);
  int16_t w = 320/3;
  int16_t h = 170/2;

  board->Lcd.setTextFont(0);
  board->Lcd.setTextSize(2);
  board->Lcd.setCursor(xPos+5, yPos+5);

  board->Lcd.print(data);
}

  /*
  if (type & 1 && type & (1 << 1)) { // Float
    double* int_data = (double*)sensor_data;
    Serial.print(*int_data, 4);
  }
  else if (type & 1) { // Signed int
    // may need to change variable type
    int* int_data = (int*)sensor_data;
    Serial.print(*int_data, DEC);
  }
  else { // Signed int
    // may need to change variable type
    unsigned int* int_data = (unsigned int*)sensor_data;
    Serial.print(*int_data, DEC);
  }/**/


// Draws a tile in the grid
void Grid::draw(int i)
{
  int16_t xPos = (320/3)*(i%3);
  int16_t yPos = 30+(170/2)*(i/3);
  int16_t w = 320/3;
  int16_t h = 170/2;
  

  if (i == block%6) {
    board->Lcd.fillRect(xPos+3,yPos+3,w-8,h-8,WHITE);
    board->Lcd.drawRect(xPos,yPos,w-2,h-2,WHITE);
  }
  else {
    board->Lcd.fillRect(xPos,yPos,w-2,h-2,BLACK);
    board->Lcd.drawRect(xPos,yPos,w-2,h-2,WHITE);
  }
}


// action upon right input
void Grid::right() {
  if (!isGraph) {
    block == pages*6-1 ? 0 : block++;
    
    if (pageNum < block/6) {
      pageNum++;
      this->begin();
    }
    else {
      draw(block%6-1);
      draw(block%6);
    }
  }
}

// action upon left input
void Grid::left() {
  if (isGraph) {
    isGraph = false;
    delete graph;
    begin();
  }
  else {
    block == 0 ? block = 0 : block--;

    if (pageNum > block/6) {
      pageNum--;
      this->begin();
    }
    else {
      draw(block%6+1);
      draw(block%6);
    }
  }
}

void Grid::record() {
  recording ? recording = false : recording = true;
  if (isGraph) {}
  else begin();
}

void Grid::select() {
  if (isGraph) {}
  else {
    isGraph = true;
    graph_pos = block;
    graph = new Graph(200, 179, board);
    graph->startGraph();
  }
}
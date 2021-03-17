#include "GraphView.h"

extern M5Stack* board;

GraphView::GraphView(UserInterface* ui_): View(ui_) {
    flag = 0;
}

int GraphView::handleKeypress() {
  // FIXME: Handle here? Or move it to the UI
  board->update();

  if (board->BtnA.wasReleased())
      return left();
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
  return 0;
}

int GraphView::end() {
    return 1;
}

int GraphView::left() {
    return end();
}

void GraphView::right() {

}

void GraphView::record() {

}

void GraphView::select() {

}

int GraphView::update() {
  // FIXME: Ditto
  int return_val = handleKeypress();
  updateGraph();
  return return_val;
}

void GraphView::begin() {
    board->Lcd.clearDisplay();
    board->Lcd.drawRect(x0, y0-_height, _width, _height, WHITE);
    // control markers
    int centerX = 320/2;
    int centerY = 225;

    board->Lcd.fillRect(0,0,316,28,WHITE);
    board->Lcd.fillRect(0,211,316,30,WHITE);
    
    // select button
    board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);
    board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);

    // L/R buttons
    board->Lcd.fillTriangle(centerX-80,centerY-8,centerX-80,centerY+8,centerX-90,centerY,BLACK);
    board->Lcd.fillTriangle(centerX+80,centerY-8,centerX+80,centerY+8,centerX+90,centerY,BLACK);
}

void GraphView::updateGraph() {
    time = millis();

    if (time % 300 == 0) {
        //int randN = 5*sin(time/100)+5;
        int randN = analogRead(36)*360/4095;
        points.push_back(randN);
        if (points.size() > pointsSize)
            points.pop_front();

        plotPoints();
        displayData();
    }
}

void GraphView::plotPoints() {
    int max;
    int min;
    double scale;

    board->Lcd.fillRect(x0+1, y0-_height+1, _width-2, _height-2, BLACK);

    //Serial.println("HELLO1");

    max = points.front();
    min = points.front();
    
    for (auto it = points.begin(); it != points.end(); ++it) {
        //Serial.println("HELLO2");
        if (*it > max)
            max = *it;
        if (*it < min)
            min = *it;
    }
    if (max == min) scale = 1;
    else scale = double((_height-10))/(max-min);

    int x_pos = 0;
    int y_pos;
    int old_y;

    for(auto it = points.begin(); it != points.end(); ++it) {
        y_pos = (*it - min)*scale+5;
        
        if (x_pos > 0) {
            board->Lcd.drawCircle(x0+x_pos, y0-y_pos, 2, WHITE);
            board->Lcd.drawLine(x0+x_pos-_width/pointsSize, y0-old_y, x0+x_pos, y0-y_pos, WHITE);
        }

        x_pos += _width/pointsSize;
        old_y = y_pos;
    }
}

void GraphView::displayData() {
    //board->Lcd.fillRect(0,0,316,28,WHITE);
    //board->Lcd.fillRect(0,211,316,30,WHITE);
    board->Lcd.setTextColor(WHITE, BLACK);
    board->Lcd.setTextFont(0);
    board->Lcd.setTextSize(2);
    board->Lcd.setCursor(225, 40);
    //if (recording)
    board->Lcd.print("VALUE:");

    board->Lcd.setTextColor(WHITE, BLACK);
    board->Lcd.setTextSize(4);
    board->Lcd.setCursor(230, 65);
    board->Lcd.print("   ");
    board->Lcd.setCursor(230, 65);
    board->Lcd.print(points.back());


    // Print time
    int curMins = (time-startTime)/60000;
    int curSecs = ((time-startTime)/1000)%60;
    String secs = timeToString(curSecs);

    board->Lcd.setTextColor(WHITE, BLACK);
    board->Lcd.setTextFont(0);
    board->Lcd.setTextSize(2);
    board->Lcd.setCursor(210, 120);
    //if (recording)
    board->Lcd.print("TIME REC:");

    board->Lcd.setTextColor(WHITE, BLACK);
    board->Lcd.setTextSize(4);
    board->Lcd.setCursor(215, 145);
    board->Lcd.print(curMins);
    board->Lcd.setCursor(235, 145);
    board->Lcd.print(":");
    board->Lcd.setCursor(255, 145);
    board->Lcd.print(secs);
    
}

String GraphView::timeToString(int num) {
    String s;

    if (num < 10)
        s = "0" + String(num);
    else
        s = String(num/10) + String(num%10);

    return s;
}
#include "Graph.h"

Graph::Graph(int width, int height, M5Stack* board) {
    _width = width;
    _height = height;
    _board = board;
    x0 = 0;
    y0 = 30+_height;
    std::srand(width);
    pointsSize = 25;
    startTime = millis();
}

void Graph::startGraph() {
    _board->Lcd.clearDisplay();
    _board->Lcd.drawRect(x0, y0-_height, _width, _height, WHITE);
    // control markers
    int centerX = 320/2;
    int centerY = 225;

    _board->Lcd.fillRect(0,0,316,28,WHITE);
    _board->Lcd.fillRect(0,211,316,30,WHITE);
    
    // select button
    _board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);
    _board->Lcd.fillRect(centerX-8,centerY-8,16,16,BLACK);

    // L/R buttons
    _board->Lcd.fillTriangle(centerX-80,centerY-8,centerX-80,centerY+8,centerX-90,centerY,BLACK);
    _board->Lcd.fillTriangle(centerX+80,centerY-8,centerX+80,centerY+8,centerX+90,centerY,BLACK);
}

void Graph::updateGraph() {
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

void Graph::plotPoints() {
    int max;
    int min;
    double scale;

    _board->Lcd.fillRect(x0+1, y0-_height+1, _width-2, _height-2, BLACK);

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

    for (auto it = points.begin(); it != points.end(); ++it) {
        y_pos = (*it - min)*scale+5;
        
        if (x_pos > 0) {
            _board->Lcd.drawCircle(x0+x_pos, y0-y_pos, 2, WHITE);
            _board->Lcd.drawLine(x0+x_pos-_width/pointsSize, y0-old_y, x0+x_pos, y0-y_pos, WHITE);
        }

        x_pos += _width/pointsSize;
        old_y = y_pos;
    }
}

void Graph::displayData() {
    //_board->Lcd.fillRect(0,0,316,28,WHITE);
    //_board->Lcd.fillRect(0,211,316,30,WHITE);
    _board->Lcd.setTextColor(WHITE, BLACK);
    _board->Lcd.setTextFont(0);
    _board->Lcd.setTextSize(2);
    _board->Lcd.setCursor(225, 40);
    //if (recording)
    _board->Lcd.print("VALUE:");

    _board->Lcd.setTextColor(WHITE, BLACK);
    _board->Lcd.setTextSize(4);
    _board->Lcd.setCursor(230, 65);
    _board->Lcd.print("   ");
    _board->Lcd.setCursor(230, 65);
    _board->Lcd.print(points.back());


    // Print time
    int curMins = (time-startTime)/60000;
    int curSecs = ((time-startTime)/1000)%60;
    String secs = timeToString(curSecs);

    _board->Lcd.setTextColor(WHITE, BLACK);
    _board->Lcd.setTextFont(0);
    _board->Lcd.setTextSize(2);
    _board->Lcd.setCursor(210, 120);
    //if (recording)
    _board->Lcd.print("TIME REC:");

    _board->Lcd.setTextColor(WHITE, BLACK);
    _board->Lcd.setTextSize(4);
    _board->Lcd.setCursor(215, 145);
    _board->Lcd.print(curMins);
    _board->Lcd.setCursor(235, 145);
    _board->Lcd.print(":");
    _board->Lcd.setCursor(255, 145);
    _board->Lcd.print(secs);
    
}

String Graph::timeToString(int num) {
    String s;

    if (num < 10)
        s = "0" + String(num);
    else
        s = String(num/10) + String(num%10);

    return s;
}
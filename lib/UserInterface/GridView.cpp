#include "GridView.h"

void GridView::handleKeypress() {
    board->update();

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
}

void GridView::update() {
  handleKeypress();
}

void GridView::begin()
{
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

    if (i == curPage)
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

  // Not sure what to do about this recording indicator
  board->Lcd.print("Ready! Hold    to record.");
  board->Lcd.fillRect(155,5,16,16,BLACK);

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

// Draws a tile in the grid
void GridView::draw(int i)
{
  int16_t xPos = (320/3)*(i%3);
  int16_t yPos = 30+(170/2)*(i/3);
  int16_t w = 320/3;
  int16_t h = 170/2;

  if (i == curCell%6) {
    board->Lcd.fillRect(xPos+3,yPos+3,w-8,h-8,WHITE);
    board->Lcd.drawRect(xPos,yPos,w-2,h-2,WHITE);
  }
  else {
    board->Lcd.fillRect(xPos,yPos,w-2,h-2,BLACK);
    board->Lcd.drawRect(xPos,yPos,w-2,h-2,WHITE);
  }
}


// action upon right input
void GridView::right() {
    curCell == pages*6-1 ? 0 : curCell++;

    if (curPage < curCell/6) {
      curPage++;
      this->begin();
    }
    else {
      draw(curCell%6-1);
      draw(curCell%6);
    }
}

// action upon left input
void GridView::left() {
    curCell == 0 ? curCell = 0 : curCell--;

    if (curPage > curCell/6) {
        curPage--;
        this->begin();
    }
    else {
        draw(curCell%6+1);
        draw(curCell%6);
    }
}

void GridView::record() {
}

void GridView::select() {
}

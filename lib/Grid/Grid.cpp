#include "Grid.h"
#include <stdlib.h>

Grid::Grid(M5Stack* other)
{
  block = 0;
  board = other;
}

void Grid::begin()
{
  for (int i = 0; i < 6; i++) {
    draw(i);
  }
}

void Grid::draw(int i)
{
  int16_t xPos = (320/3)*(i%3);
  int16_t yPos = (240/2)*(i/3);
  int16_t w = 320/3;
  int16_t h = 240/2;

  if (i == block) {
    board->Lcd.fillRect(xPos,yPos,w-2,h-2,WHITE);
  }
  else {
    board->Lcd.fillRect(xPos,yPos,w-2,h-2,BLACK);
    board->Lcd.drawRect(xPos,yPos,w-2,h-2,WHITE);
  }
}

void Grid::right() {
  block++;

  draw(block-1);
  draw(block);
}

void Grid::left() {
  block == 0 ? block = 0 : block--;

  draw(block+1);
  draw(block);
}

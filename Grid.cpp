#include "Grid.h"

Grid::Grid()
{
  block = 0;

  for (int i = 0; i < 6; i++) {
    draw(i);
  }
}

void Grid::draw(int i) {
  int16_t xPos = (320/3)*(i%3);
  int16_t yPos = (240/2)*(i/3);
  int16_t w = 320/3;
  int16_t h = 240/2;

  if (i == block) {
    fillRect(xPos,yPos,w,h,WHITE);
  }
  else {
    fillRect(xPos,yPos,w,h,BLACK);
    drawRect(xPos,yPos,w,h,WHITE);
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

#include "UserInterface.h"

UserInterface::UserInterface(M5Stack* stack) {
    board = stack;
}

void UserInterface::begin() {
    permanentViews[0] = new GridView(board);
    activeView = 0;
    permanentViews[activeView]->begin();
}

void UserInterface::update() {
    permanentViews[activeView]->update();
}

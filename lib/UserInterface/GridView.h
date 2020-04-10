#pragma once
#include <UserInterface.h>

class GridView : public View {
    void begin();
    void update();
    void end();
    void draw(int i);
    void handleKeypress();
    int8_t totalPages(); // f(sensor count)
    int8_t curPage = 0;
    uint8_t curCell;
    uint8_t pages = 6;
    private:
    void left();
    void right();
    void record();
    void select();
    UserInterface* ui;
    int flag;
};

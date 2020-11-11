#pragma once
#include "UserInterface.h"

class GridView : public View {
    public:
    GridView(UserInterface* ui_): View(ui_) {};
    void begin();
    void update();
    void end();
    void draw(int i);
    void handleKeypress();
    int8_t totalPages(); // f(sensor count)
    int8_t curPage = 0;
    uint8_t curCell = 0;
    uint8_t pages = 6;
    private:
    void left();
    void right();
    void record();
    void select();

    int flag;
};

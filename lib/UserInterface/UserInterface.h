#pragma once

#include <M5Stack.h>
#include <Collector.h>
#include <GridView.h>

class View {
    public:
    View(M5Stack* stack);
    virtual void begin();
    virtual void update();
    virtual void end();
    virtual void handleKeypress();

    M5Stack* board;
};

View::View(M5Stack* stack) {
    board = stack;
}

class UserInterface {
    public:
    UserInterface(M5Stack* stack);
    void begin();
    void update();
    // Handling keyboard
    // Handling redraw
    View* permanentViews[2];
    uint8_t activeView;
    void enterGridView(int page);  // new View here, pass to switchView
    void enterGraphView(SensorEntry* entry);
    private:
    void switchView(View* newView);
    /*
        if (activeView) {
            activeView->end();
            delete activeView;
        }
        activeView = newView;
        newView->begin();
    */
    uint8_t lastGraphViewPage = 0;
    M5Stack* board;
};


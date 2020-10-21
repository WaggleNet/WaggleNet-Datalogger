#pragma once

#include <M5Stack.h>
#include <Collector.h>

#include <vector>

using namespace std;

class UserInterface;

class View {
    public:
    View(UserInterface* ui_);
    virtual void begin() = 0;
    virtual void update() = 0;
    virtual void end() = 0;
    virtual void handleKeypress() = 0;
    UserInterface* ui;

    // M5Stack* board;
};

class UserInterface {
    public:
    UserInterface(M5Stack* stack);
    void begin(int defaultApp);
    void update();
    // Handling keyboard
    // Handling redraw
    /* Widgets are permanent views that always draw last*/
    vector<View*> widgetViews;
    /* Apps are views only active one at a time */
    vector<View*> appViews;
    int activeAppView = -1; // Default -1 for protection
    void addWidget(View* view_);
    void addApp(View* view_);
    void switchApp(int8_t view_idx);
    
    private:
    M5Stack* board;
};


#include "UserInterface.h"

UserInterface::UserInterface(M5Stack* stack)
: board(stack){

}

void UserInterface::begin(int defaultApp) {
    Serial.print("hello");
    // activeAppView = defaultApp;
    Serial.print((long)appViews.back(), HEX);
    appViews.back()->begin();
    // for (auto const& view: widgetViews) {
        // view->begin();
    // }
}

void UserInterface::update() {
    // If value 1 is returned, then we know program is terminated
    int return_val = appViews.back()->update();
    // Remove current program if it has terminated
    if (return_val == 1) {
        appViews.pop_back();
        // Restart old program
        appViews.back()->begin();
    }
}

void UserInterface::addApp(View* view_) {
    Serial.print("Adding app");
    appViews.push_back(view_);
    appViews.back()->begin();
}

void UserInterface::addWidget(View* view_) {
    widgetViews.push_back(view_);
}

View::View(UserInterface* ui_): ui(ui_) {};

#include "UserInterface.h"

UserInterface::UserInterface(M5Stack* stack)
: board(stack){

}

void UserInterface::begin(int defaultApp) {
    activeAppView = defaultApp;
    appViews[defaultApp]->begin();
    for (auto const& view: widgetViews) {
        view->begin();
    }
}

void UserInterface::update() {
    appViews[activeAppView]->update();
    // TODO: Handle keypress here?
    // How about holds?
}

void UserInterface::addApp(View* view_) {
    appViews.push_back(view_);
}

void UserInterface::addWidget(View* view_) {
    widgetViews.push_back(view_);
}

View::View(UserInterface* ui_): ui(ui_) {};

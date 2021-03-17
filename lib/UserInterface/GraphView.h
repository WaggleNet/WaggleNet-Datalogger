#pragma once
#include "UserInterface.h"

class GraphView : public View {
    public:
        GraphView(UserInterface* ui_);
        void begin();
        int update();
        int end();
        void draw(int i);
        int handleKeypress();
    private:
        int left();
        void right();
        void record();
        void select();

        void updateGraph();
        void plotPoints();
        void displayData();
        String timeToString(int num);

        std::list<int> points;
        int _width;
        int _height;
        int x0, y0;
        int pointsSize;
        unsigned long time;
        unsigned long startTime;

    int flag;
};
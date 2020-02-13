#include <M5Stack.h>
#include <list>

class Graph
{
    private:
        std::list<float> points;
        int _width;
        int _height;
        int x0, y0;
        M5Stack* _board;
        int pointsSize;
        unsigned long time;
        unsigned long startTime;

    public:
        Graph(int width, int height, M5Stack* board);
        //~Graph();

        void startGraph();
        void updateGraph(float data);
        void plotPoints();
        void displayData();
        String timeToString(int num);
};
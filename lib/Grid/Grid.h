#include <M5Stack.h>
#include <Graph.h>
#include <cstdlib>
#include <pnpmanager.h>

class Grid
{
  private:
    int block;
    int pageNum;
    int pages;
    int flag;
    bool recording;
    bool isGraph;
    M5Stack* board;
    Graph* graph;
    SensorManager* manager;

  public:
    Grid(M5Stack* other);

    void begin();
    void draw(int i);
    void right();
    void left();
    void select();
    void record();
    void update();
    void updateSensors();
    void drawSensorVal(int idx);

};

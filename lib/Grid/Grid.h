#include <M5Stack.h>
#include <Graph.h>
#include <cstdlib>

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

  public:
    Grid(M5Stack* other);

    void begin();
    void draw(int i);
    void right();
    void left();
    void select();
    void record();
    void update();

};

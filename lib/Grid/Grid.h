#include <M5Stack.h>

class Grid
{
  private:
    int block;
    M5Stack board;

  public:
    Grid(M5Stack& other);

    void begin();
    void draw(int i);
    void right();
    void left();
    // void select();
    // void record();

};

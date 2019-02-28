#include <M5Stack.h>

class Grid
{
  private:
    int block;
    int pageNum;
    int pages;
    bool recording;
    M5Stack* board;

  public:
    Grid(M5Stack* other);

    void begin();
    void draw(int i);
    void right();
    void left();
    // void select();
    void record();

};

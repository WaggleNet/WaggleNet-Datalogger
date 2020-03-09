#include <M5Stack.h>
#include <ezTime.h>


class Timekeeper {
    public:
    Timekeeper(M5Stack* m5);
    void begin();
    void update();

    private:
    M5Stack* board;
};

extern Timekeeper time;

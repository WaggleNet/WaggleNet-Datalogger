#pragma once
#include <M5Stack.h>
#include <ezTime.h>

#include "Config.h"


class Timekeeper {
    public:
    Timekeeper(M5Stack* m5);
    void begin();
    void update();

    void updateNTPSettings();
    time_t getNow();
    void setNow(time_t newnow);

    private:
    M5Stack* board;
};

extern Timekeeper timekeeper;

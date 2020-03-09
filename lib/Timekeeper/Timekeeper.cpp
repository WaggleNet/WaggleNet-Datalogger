#include "Timekeeper.h"

Timekeeper::Timekeeper(M5Stack* m5):board(m5) {
    
}

void Timekeeper::updateNTPSettings() {
    setServer(config::get(CONF_NTP_SERVER));
}

void Timekeeper::setNow(time_t newnow) {
    setTime(newnow);
}

time_t Timekeeper::getNow() {
    return now();
}


void Timekeeper::begin() {
    
}

void Timekeeper::update() {
    events();
}

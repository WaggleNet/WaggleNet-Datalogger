#include "Timekeeper.h"

Timekeeper::Timekeeper(M5Stack* m5):board(m5) {
    
}


void Timekeeper::begin() {
    
}

void Timekeeper::update() {
    events();
}

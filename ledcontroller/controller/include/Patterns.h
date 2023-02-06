#ifndef PATTERNS_H_
#define PATTERNS_H_

#include <Arduino.h>

enum class PatternType {
    None = 0,
    SetAll = 1,
    Blink = 2,
    RGBFade = 3,
    HackerMode = 4,
    Chase = 5,
    Wipe = 6
};

struct Pattern {
    PatternType type;
    uint16_t numStates;
    uint16_t changeDelay;
}

static Pattern patterns[] = {
    {
        .type = PatternType::Blink,
        .numStates = 2,
        .changeDelay = 3000u
    },
    {
        
    }
}



#endif
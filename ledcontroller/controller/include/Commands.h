#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <Arduino.h>

enum class CommandType { On = 0, Off = 1, Pattern = 2, ChangeColor = 3, ReadPatternDone = 4 };

struct CommandOn {};

struct CommandOff {};

struct CommandPattern {
    uint8_t pattern;
    uint8_t oneShot;
};

struct CommandColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct CommandReadPatternDone {};

union CommandData {
    CommandOn commandOn;
    CommandOff commandOff;
    CommandPattern commandPattern;
    CommandColor commandColor;
    CommandReadPatternDone commandReadPatternDone;
};

struct Command {
    CommandType commandType;
    CommandData commandData;
};

#endif
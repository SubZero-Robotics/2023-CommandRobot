#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <Arduino.h>

enum class CommandType { On = 0, Off = 1, Pattern = 2, ChangeColor = 3 };

struct CommandOn {};

struct CommandOff {};

struct CommandPattern {
    uint8_t pattern;
};

struct CommandColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

union CommandData {
    CommandOn commandOn;
    CommandOff commandOff;
    CommandPattern commandPattern;
    CommandColor commandColor;
};

struct Command {
    CommandType commandType;
    CommandData commandData;
};

#endif
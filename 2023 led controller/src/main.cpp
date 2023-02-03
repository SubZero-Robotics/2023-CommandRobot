#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#define PIN 2
#define LEDNUM 150

// Forward declaration
void receiveEvent(int);


enum class CommandType {
   On = 0,
   Off = 1,
   Pattern = 2,
   ChangeColor = 3
};

struct CommandOn {

};

struct CommandOff {

};

struct CommandPattern {
    uint8_t pattern;
};

struct CommandColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    // uint8_t colors[3];
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

static Command command;

constexpr uint8_t slaveAddress = 0x01;

void setup() {
    Wire.begin(slaveAddress);     // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output
}

void loop() {
    delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
// 
}
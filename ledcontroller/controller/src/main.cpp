#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include "Commands.h"

// Forward declaration
void receiveEvent(int);

constexpr uint8_t receiveBufSize = 8;
constexpr uint8_t slaveAddress = 0x01;
constexpr uint8_t ledDataOutPin = 2;
constexpr uint16_t ledNum = 150;
constexpr uint8_t ledBrightness = 100;

CRGB leds[ledNum];

static volatile uint8_t receiveBuf[receiveBufSize];
static volatile bool newData = false;
static Command command;
static CRGB currentColor;

void setAllLEDs(CRGB color) {
    for (size_t i = 0; i < ledNum; i++) {
        leds[i] = color;
    }

    FastLED.show();
}

void setup() {
    Wire.begin(slaveAddress);     // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output
    FastLED.addLeds<WS2812B, ledDataOutPin, BGR>(leds, ledNum);
    FastLED.setBrightness(ledBrightness);
    setAllLEDs(CRGB::Black);
}

void loop() {
    // If there's new data, process it
    if (newData) {
        uint8_t buf[receiveBufSize];
        noInterrupts();
        memcpy(buf, (const void*)receiveBuf, receiveBufSize);
        parseCommand(buf, receiveBufSize);
        newData = false;
        switch (command.commandType)
        {
        case CommandType::On:
            // Go back to the last color and pattern instead
            setAllLEDs(CRGB::White);
            break;

        case CommandType::Off:
            setAllLEDs(CRGB::Black);
            break;

        case CommandType::Pattern:
            // To set everything to a certain color, change color then call the 'set all' pattern
            // Create named pattern types and a method for running each pattern
            break;

        case CommandType::ChangeColor:
            auto colors = command.commandData.commandColor;
            currentColor = CRGB(colors.red, colors.green, colors.blue);
            break;
        default:
            break;
        }

        interrupts();
    }
   // What do we do with the data in command?
}

// function to take data from buf and populate command

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    Wire.readBytes((uint8_t*)receiveBuf, howMany);
    newData = true;
}

void parseCommand(uint8_t *buf, size_t len) {
    auto type = (CommandType)buf[0];
    command.commandType = type;
    switch (type) {
    case CommandType::On :
        command.commandData.commandOn = { };
        break;

    case CommandType::Off :
        command.commandData.commandOff = { };
        break;

    case CommandType::Pattern :
        command.commandData.commandPattern = {.pattern = buf[1]};
        break;

    case CommandType::ChangeColor :
        command.commandData.commandColor;
        memcpy(&command.commandData.commandColor.red, &buf[1], 3);
        break;

    default:
        break;
    }
    
}
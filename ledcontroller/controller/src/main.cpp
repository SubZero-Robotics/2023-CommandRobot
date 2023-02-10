#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#include "Commands.h"
#include "Patterns.h"

// Forward declaration
void receiveEvent(int);
void parseCommand(uint8_t *, size_t);
// The function type comes from ExecutePatternCallback in Patterns.h
bool executePatternNone(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternSetAll(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternBlink(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternRGBFade(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternHackerMode(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternChase(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternWipe(CRGB *, CRGB, uint16_t, uint16_t);
// Forward declares new patterns here

constexpr uint8_t receiveBufSize = 8;
constexpr uint8_t slaveAddress = 0x01;
constexpr uint8_t ledDataOutPin = 2;
constexpr uint8_t ledBrightness = 100;

static CRGB leds[ledNum];
// ! The order of these MUST match the order in PatternType!
// TODO: Fill the rest of these
static Pattern patterns[patternCount] = {
    {.type = PatternType::None,
     .numStates = 0,
     .changeDelay = 0,
     .cb = &executePatternNone},
    {.type = PatternType::SetAll,
     .numStates = 0,
     .changeDelay = 20000u,
     .cb = &executePatternSetAll},
    {.type = PatternType::Blink,
     .numStates = 2,
     .changeDelay = 3000u,
     .cb = &executePatternBlink},
    {.type = PatternType::RGBFade,
     .numStates = 256,
     .changeDelay = 5u,
     .cb = &executePatternRGBFade},
    {.type = PatternType::HackerMode,
     .numStates = 2,
     .changeDelay = 3000u,
     .cb = &executePatternHackerMode},
    {.type = PatternType::Chase,
     .numStates = ledNum + chaseLEDWidth - 1,
     .changeDelay = 25u,
     .cb = &executePatternChase},
    {.type = PatternType::Wipe,
     .numStates = ledNum,
     .changeDelay = 25u,
     .cb = &executePatternWipe}};

static volatile uint8_t receiveBuf[receiveBufSize];
static volatile bool newData = false;
static Command command;
static CRGB currentColor;
static PatternRunner patternRunner(&FastLED, patterns);

// TODO: Convert this to a pattern callback
void setAllLEDs(CRGB color) {
    for (size_t i = 0; i < ledNum; i++) {
        leds[i] = color;
    }

    FastLED.show();
}

void setup() {
    Wire.begin(slaveAddress);      // join i2c bus with address #4
    Wire.onReceive(receiveEvent);  // register event
    Serial.begin(9600);            // start serial for output
    FastLED.addLeds<WS2812B, ledDataOutPin, BGR>(leds, ledNum);
    FastLED.setBrightness(ledBrightness);
}

void loop() {
    // If there's new data, process it
    if (newData) {
        uint8_t buf[receiveBufSize];
        noInterrupts();
        memcpy(buf, (const void *)receiveBuf, receiveBufSize);
        parseCommand(buf, receiveBufSize);
        newData = false;
        switch (command.commandType) {
            case CommandType::On:
                // TODO: Go back to the last color and pattern
                break;

            case CommandType::Off:
                // TODO: Replace this with a call to the SetAll pattern
                setAllLEDs(CRGB::Black);
                break;

            case CommandType::Pattern:
                // To set everything to a certain color, change color then call
                // the 'set all' pattern
                patternRunner.setCurrentPattern(
                    command.commandData.commandPattern.pattern);
                break;

            case CommandType::ChangeColor:
                auto colors = command.commandData.commandColor;
                currentColor = CRGB(colors.red, colors.green, colors.blue);
                patternRunner.setCurrentColor(currentColor);
                break;
        }

        interrupts();
    }
    // What do we do with the data in command?
    patternRunner.update();
}

// function to take data from buf and populate command

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    Wire.readBytes((uint8_t *)receiveBuf, howMany);
    newData = true;
}

void parseCommand(uint8_t *buf, size_t len) {
    auto type = (CommandType)buf[0];
    command.commandType = type;
    switch (type) {
        case CommandType::On:
            command.commandData.commandOn = {};
            break;

        case CommandType::Off:
            command.commandData.commandOff = {};
            break;

        case CommandType::Pattern:
            command.commandData.commandPattern = {.pattern = buf[1]};
            break;

        case CommandType::ChangeColor:
            memcpy(&command.commandData.commandColor.red, &buf[1], 3);
            break;

        default:
            break;
    }
}

bool executePatternNone(CRGB *leds, CRGB color, uint16_t state,
                        uint16_t ledCount) {
    return false;
}

bool executePatternSetAll(CRGB *leds, CRGB color, uint16_t state,
                          uint16_t ledCount) {
    for (size_t i = 0; i < ledCount; i++) {
        leds[i] = color;
    }
    return true;
}

bool executePatternBlink(CRGB *leds, CRGB color, uint16_t state,
                         uint16_t ledCount) {
    switch (state) {
        case 0:
            for (size_t i = 0; i < ledCount; i++) {
                leds[i] = color;
            }
            return true;

        case 1:
            for (size_t i = 0; i < ledCount; i++) {
                leds[i] = CRGB::Black;
            }
            return true;

        default:
            return false;
    }
}

bool executePatternRGBFade(CRGB *leds, CRGB color, uint16_t state,
                           uint16_t ledCount) {
    for (size_t i = 0; i < ledCount; i++) {
        leds[i] = PatternRunner::Wheel(((i * 256 / ledCount) + state) & 255);
    }
    return true;
}

bool executePatternHackerMode(CRGB *leds, CRGB color, uint16_t state,
                              uint16_t ledCount) {
    switch (state) {
        case 0:
            return executePatternSetAll(leds, CRGB::Green, 0, ledCount);

        case 1:
            return executePatternSetAll(leds, CRGB::DarkGreen, 0, ledCount);

        default:
            return false;
    }
}

bool executePatternChase(CRGB *leds, CRGB color, uint16_t state,
                         uint16_t ledCount) {
    if (state - 1 >= 0) {
        leds[state - 1] = CRGB::Black;
    }
    for (size_t i = state; i < state + chaseLEDWidth; i++) {
        if (i < ledCount) {
            leds[i] = color;
        }
    }
    return true;
}

bool executePatternWipe(CRGB *leds, CRGB color, uint16_t state,
                        uint16_t ledCount) {
    leds[state] = color;
    return true;
}

// TODO: Implement your pattern callbacks here
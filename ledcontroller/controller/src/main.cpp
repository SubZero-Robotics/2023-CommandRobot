#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#include "Commands.h"
#include "Patterns.h"

// Forward declarations
void receiveEvent(int);
void requestEvent(void);
void parseCommand(uint8_t *, size_t);
// The function signature comes from ExecutePatternCallback in Patterns.h
// Forward declare new patterns here
bool executePatternNone(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternSetAll(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternBlink(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternRGBFade(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternHackerMode(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternChase(CRGB *, CRGB, uint16_t, uint16_t);
bool executePatternWipe(CRGB *, CRGB, uint16_t, uint16_t);

constexpr uint8_t receiveBufSize = 8;
constexpr uint8_t slaveAddress = 0x01;
constexpr uint8_t ledDataOutPin = 2;
constexpr uint8_t ledBrightness = 100;

// ! The order of these MUST match the order in PatternType!
static Pattern patterns[patternCount] = {
    {.type = PatternType::None,
     .numStates = 0,
     .changeDelay = 0,
     .cb = executePatternNone},
    {.type = PatternType::SetAll,
     .numStates = 0,
     .changeDelay = 20000u,
     .cb = executePatternSetAll},
    {.type = PatternType::Blink,
     .numStates = 2,
     .changeDelay = 3000u,
     .cb = executePatternBlink},
    {.type = PatternType::RGBFade,
     .numStates = 256,
     .changeDelay = 5u,
     .cb = executePatternRGBFade},
    {.type = PatternType::HackerMode,
     .numStates = 2,
     .changeDelay = 3000u,
     .cb = executePatternHackerMode},
    {.type = PatternType::Chase,
     .numStates = ledNum + chaseLEDWidth - 1,
     .changeDelay = 25u,
     .cb = executePatternChase},
    {.type = PatternType::Wipe,
     .numStates = ledNum,
     .changeDelay = 25u,
     .cb = executePatternWipe}};

static volatile uint8_t receiveBuf[receiveBufSize];
static volatile bool newData = false;
static Command command;
static CRGB currentColor;
static CRGB leds[ledNum];
static PatternRunner patternRunner(&FastLED, patterns);
static bool systemOn = true;

void setup() {
    Wire.begin(slaveAddress);      // join i2c bus with address #4
    Wire.onReceive(receiveEvent);  // register event
    Wire.onRequest(requestEvent);
    Serial.begin(115200);  // start serial for output
    FastLED.addLeds<WS2812B, ledDataOutPin, BGR>(leds, ledNum);
    FastLED.setBrightness(ledBrightness);

    // Initialize all LEDs to black
    executePatternSetAll(leds, CRGB::Black, 0, ledNum);
    FastLED.show();
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
                // Go back to running the current color and pattern
                patternRunner.reset();
                systemOn = true;
                break;

            case CommandType::Off:
                // Set LEDs to black and stop running the pattern
                executePatternSetAll(leds, CRGB::Black, 0, ledNum);
                FastLED.show();
                systemOn = false;
                break;

            case CommandType::Pattern:
                // To set everything to a certain color, change color then call
                // the 'set all' pattern
                patternRunner.setCurrentPattern(
                    command.commandData.commandPattern.pattern,
                    command.commandData.commandPattern.oneShot);
                break;

            case CommandType::ChangeColor: {
                auto colors = command.commandData.commandColor;
                currentColor = CRGB(colors.red, colors.green, colors.blue);
                patternRunner.setCurrentColor(currentColor);
            } break;

            case CommandType::ReadPatternDone:
                break;

            default:
                break;
        }

        interrupts();
    }

    if (systemOn) {
        patternRunner.update();
    }
}

void receiveEvent(int howMany) {
    Wire.readBytes((uint8_t *)receiveBuf, howMany);
    newData = true;
}

void requestEvent() {
    switch (command.commandType) {
        case CommandType::ReadPatternDone:
            Wire.write(patternRunner.patternDone());
            break;

        default:
            // Send back 255 (-1 signed) to indicate bad/no data
            Wire.write(0xff);
    }
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
            memcpy(&command.commandData.commandPattern.pattern, &buf[1], 2);
            break;

        case CommandType::ChangeColor:
            memcpy(&command.commandData.commandColor.red, &buf[1], 3);
            break;

        case CommandType::ReadPatternDone:
            command.commandData.commandReadPatternDone = {};
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
#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

constexpr uint8_t slaveAddr = 0x08;
constexpr uint16_t commandDelayMs = 200;

enum class InputOptions {
    RunTests = 0,
    SetOn = 1,
    SetOff = 2,
    SetColor = 3,
    SetPattern = 4,
    ReadPatternDone = 5,
    Invalid = 6
};

enum class CommandType {
    CommandOn = 0,
    CommandOff = 1,
    CommandPattern = 2,
    CommandColor = 3,
    CommandReadPatternDone = 4
};

enum class PatternType {
    None = 0,
    SetAll = 1,
    Blink = 2,
    RGBFade = 3,
    HackerMode = 4,
    Chase = 5,
    Wipe = 6
};

void writeCommand(CommandType type, uint8_t *data);
void setPattern(PatternType pattern, uint8_t oneShot);
void setColor(CRGB color);
void setOnOff(bool isOn);

void printMenu();
InputOptions parseInputOption(char);
void handleInput(InputOptions);
void handleRunMainTests();
void handleOnOff(bool val);
void handleColor();
void handlePattern();
void handleReadPatternDone();

void setup() {
    Serial.begin(115200);
    Wire.begin();
    setOnOff(false);
    Serial.println(F("Ready. Set your serial monitor to send Newline"));
}

void loop() {
    printMenu();
    while (!Serial.available())
        ;
    String input = Serial.readStringUntil('\n');
    input.trim();
    InputOptions option = parseInputOption(input.charAt(0));
    if (option == InputOptions::Invalid)
        ;
    else {
        handleInput(option);
    }
    Serial.println();
}

void runMainTests() {
    // What happens if we set the color while it's off?
    Serial.println(F("Setting color to blue"));
    setColor(CRGB::Blue);
    Serial.println(F("Setting to on"));
    setOnOff(true);
    delay(1000);
    Serial.println(F("Setting color to purple"));
    setColor(CRGB::Purple);
    delay(1000);
    Serial.println(F("Setting color to yellow"));
    setColor(CRGB::Yellow);
    delay(1000);
    Serial.println(F("Setting to off"));
    setOnOff(false);
    // Should maintain yellow color
    Serial.println(F("Setting to on"));
    setOnOff(true);
    setColor(CRGB::ForestGreen);
    Serial.println(F("Setting pattern to set all"));
    setPattern(PatternType::SetAll, true);
    delay(1000);
    Serial.println(F("Setting pattern to blink"));
    setPattern(PatternType::Blink, false);
    delay(3000);
    Serial.println(F("Setting color to blue"));
    setColor(CRGB::Blue);
    Serial.println(F("Setting pattern to chase"));
    setPattern(PatternType::Chase, false);
    delay(3000);
    Serial.println(F("Setting color to cyan"));
    setColor(CRGB::Cyan);
    delay(1000);
    Serial.println(F("Setting pattern to hacker mode"));
    setPattern(PatternType::HackerMode, false);
    delay(4000);
    Serial.println(F("Setting pattern to RGB fade"));
    setPattern(PatternType::RGBFade, false);
    delay(5000);
    Serial.println(F("Setting color to magenta"));
    setColor(CRGB::Magenta);
    Serial.println(F("Setting pattern to one-shot wipe"));
    setPattern(PatternType::SetAll, true);
    delay(4000);
    setOnOff(false);
    delay(1000);
    Serial.println(F("Main tests done"));
}

void printMenu() {
    Serial.println(
        F("Pick ONE option from the available commands. Send as single "
          "character:"));
    Serial.println(F("\t't' - Run test sequence"));
    Serial.println(F("\t'c' - Set the color"));
    Serial.println(F("\t'Y' - Set to on"));
    Serial.println(F("\t'N' - Set to off"));
    Serial.println(F("\t'p' - Set to a pattern"));
    Serial.println(F("\t'd' - Read if pattern is done running"));
}

InputOptions parseInputOption(char input) {
    switch (input) {
        case 't':
            return InputOptions::RunTests;

        case 'c':
            return InputOptions::SetColor;

        case 'Y':
            return InputOptions::SetOn;

        case 'N':
            return InputOptions::SetOff;

        case 'p':
            return InputOptions::SetPattern;

        case 'd':
            return InputOptions::ReadPatternDone;
    }

    Serial.print(F("Invalid option "));
    Serial.println(input);
    return InputOptions::Invalid;
}

void handleInput(InputOptions option) {
    uint8_t optionInt = (uint8_t)option;

    switch (option) {
        case InputOptions::RunTests:
            handleRunMainTests();
            break;

        case InputOptions::SetOn:
        case InputOptions::SetOff:
            handleOnOff(optionInt - 1);
            break;

        case InputOptions::SetColor:
            handleColor();
            break;

        case InputOptions::SetPattern:
            handlePattern();
            break;

        case InputOptions::ReadPatternDone:
            handleReadPatternDone();
            break;

        case InputOptions::Invalid:
        default:
            Serial.println(F("Invalid option"));
    }
}

void handleRunMainTests() {
    Serial.println(F("Running main tests..."));
    runMainTests();
}

// Val == 0 means ON, 1 is OFF
void handleOnOff(bool val) {
    if (!val) {
        Serial.println(F("Setting to ON"));
    } else {
        Serial.println(F("Setting to OFF"));
    }

    setOnOff(!val);
}

void handleColor() {
    Serial.println(
        F("Enter a color in the form of: R,G,B; where values are in decimal "
          "format. Don't forget the semicolon at the end!"));
    Serial.println(F("Example: 50,100,255;"));
    while (!Serial.available())
        ;
    uint8_t r = Serial.readStringUntil(',').toInt();
    uint8_t g = Serial.readStringUntil(',').toInt();
    uint8_t b = Serial.readStringUntil(';').toInt();
    Serial.print(F("Writing color: "));
    Serial.print(r);
    Serial.print(',');
    Serial.print('g');
    Serial.print(',');
    Serial.println(b);

    uint8_t buf[3] = {r, g, b};
    writeCommand(CommandType::CommandColor, buf);
}

void handlePattern() {
    Serial.println(F("Enter the desired pattern:"));
    Serial.println(F("\t'n' - None"));
    Serial.println(F("\t'a' - Set all"));
    Serial.println(F("\t'b' - Blink"));
    Serial.println(F("\t'f' - RGB fade"));
    Serial.println(F("\t'h' - Hacker mode"));
    Serial.println(F("\t'c' - Chase"));
    Serial.println(F("\t'w' - Wipe"));

    PatternType pattern = PatternType::None;
    String input = Serial.readStringUntil('\n');
    input.trim();
    char option = input.charAt(0);
    switch (option) {
        case 'a':
            pattern = PatternType::SetAll;
            break;

        case 'b':
            pattern = PatternType::Blink;
            break;

        case 'f':
            pattern = PatternType::RGBFade;
            break;

        case 'h':
            pattern = PatternType::HackerMode;
            break;

        case 'c':
            pattern = PatternType::Chase;
            break;

        case 'w':
            pattern = PatternType::Wipe;
            break;

        case 'n':
        default:
            pattern = PatternType::None;
    }

    Serial.print(F("Run once (one-shot)? (Y/N): "));
    while (!Serial.available())
        ;
    input = Serial.readStringUntil('\n');
    input.trim();
    uint8_t oneShot = input.charAt(0) == 'Y' ? 1 : 0;
    setPattern(pattern, oneShot);
}

void handleReadPatternDone() {
    Serial.println(F("Reading if pattern is done"));
    Wire.beginTransmission(slaveAddr);
    Wire.write((uint8_t)CommandType::CommandReadPatternDone);
    Wire.endTransmission();
    Wire.requestFrom(slaveAddr, 1u);
    uint8_t done = Wire.read();
    Serial.print("Device returned ");
    Serial.println(done);
}

void setPattern(PatternType pattern, uint8_t oneShot) {
    uint8_t buf[2] = {(uint8_t)pattern, oneShot};
    writeCommand(CommandType::CommandPattern, buf);
}

void setColor(CRGB color) {
    uint8_t buf[] = {color.r, color.g, color.b};
    writeCommand(CommandType::CommandColor, buf);
}

void setOnOff(bool isOn) {
    if (isOn) {
        writeCommand(CommandType::CommandOn, nullptr);
        return;
    }

    writeCommand(CommandType::CommandOff, nullptr);
}

void writeCommand(CommandType type, uint8_t *data) {
    Wire.beginTransmission(slaveAddr);
    Wire.write((uint8_t)type);

    switch (type) {
        case CommandType::CommandOn:
        case CommandType::CommandOff:
            break;

        case CommandType::CommandPattern:
            Wire.write(data[0]);  // pattern
            Wire.write(data[1]);  // one-shot
            break;

        case CommandType::CommandColor:
            for (uint8_t i = 0; i < 3; i++) {
                Wire.write(data[i]);  // RGB color bytes
            }
            break;

        default:
            break;
    }

    Wire.endTransmission();

    delay(commandDelayMs);
}
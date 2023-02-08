#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

constexpr uint8_t slaveAddr = 0x08;
constexpr uint16_t commandDelayMs = 1000;

enum class CommandType {
    CommandOn = 0,
    CommandOff = 1,
    CommandPattern = 2,
    CommandColor = 3
};

void setup() {
    Serial.begin(115200);
    Wire.begin();
    setOnOff(false);
    Serial.println("Ready");
}

void loop() {
    // What happens if we set the color while it's off?
    Serial.println("Setting color to blue");
    setColor(CRGB::Blue);
    Serial.println("Setting color to purple");
    setColor(CRGB::Purple);
    Serial.println("Setting color to yellow");
    setColor(CRGB::Yellow);
    Serial.println("Setting to off");
    setOnOff(false);
    // Should maintain yellow color
    Serial.println("Setting to on");
    setOnOff(true);
    // TODO
    Serial.println("Setting pattern to ");
    Serial.println("Setting pattern to ");
    Serial.println("Setting pattern to ");
    Serial.println("Setting color to ");
    Serial.println("Setting pattern to ");
    Serial.println("Setting color to ");
    Serial.println("Setting pattern to ");
    setOnOff(false);
}

// TODO: Convert pattern to named type
void setPattern(uint8_t pattern) {
    writeCommand(CommandType::CommandPattern, &pattern);
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
            Wire.write(data[0]);
            break;

        case CommandType::CommandColor:
            for (uint8_t i = 0; i < 3; i++) {
                Wire.write(data[i]);
            }
            break;

        default:
            break;
    }

    Wire.endTransmission();

    delay(commandDelayMs);
}
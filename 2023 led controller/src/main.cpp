#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#define PIN 2
#define LEDNUM 150

// Forward declaration
void receiveEvent(int);

/*
enum class CommandType
*/

struct RGBColorData {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    // uint8_t colors[3];
};

union CommandData {
    uint8_t pattern;
    RGBColorData color;
};

struct Command {
    uint8_t command;
    CommandData commandData;
};

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
    while(1 < Wire.available()) // loop through all but the last
    {
      char c = Wire.read(); // receive byte as a character
      Serial.print(c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);         // print the integer
}
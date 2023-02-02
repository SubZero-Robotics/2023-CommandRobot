#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
  
#define PIN 6
#define LEDNUM 150
struct RGB
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};


typedef struct Command
{
  uint8_t command;
  union
  {
    uint8_t pattern;
    RGB color;
  }
};

constexpr uint8_t slaveAddress = 0x01;

void setup()
{
    Wire.begin(slaveAddress);     // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while(1 < Wire.available()) // loop through all but the last
    {
      char c = Wire.read(); // receive byte as a character
      Serial.print(c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);         // print the integer
}
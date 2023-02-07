#ifndef PATTERNS_H_
#define PATTERNS_H_

#include <Arduino.h>
#include <FastLED.h>

/**
 * Will be called after set delay has passed
 * @param state resets to 0 after current state >= numStates
 * @returns true if LEDs should show
*/
typedef bool (*ExecutePatternCallback)(CRGB *leds, CRGB color, uint16_t state, uint16_t ledCount);
constexpr uint8_t patternCount = 7;

enum class PatternType {
    None = 0,
    SetAll = 1,
    Blink = 2,
    RGBFade = 3,
    HackerMode = 4,
    Chase = 5,
    Wipe = 6
};

struct Pattern {
    PatternType type;
    uint16_t numStates;
    uint16_t changeDelay;
    ExecutePatternCallback cb;
};

class PatternRunner {
    public:
        PatternRunner(CFastLED *fastLed, Pattern *patterns, uint8_t numPatterns = patternCount) :
        _fastLed(fastLed), _patternArr(patterns), _numPatterns(numPatterns), _curPattern(0),
        _curState(0), _curColor(CRGB::Black), _lastUpdate(0) {

        }

        /**
         * Call this every in every iteration of loop
        */
        void update();

        void setCurrentColor(CRGB color) {
            _curColor = color;
        }

        /**
         * Set the current pattern
         * @returns true if successful
        */
        bool setCurrentPattern(PatternType type);
        bool setCurrentPattern(uint8_t);

        /**
         * Get pointer to the current pattern
        */
        Pattern* currentPattern() const;

        /**
         * Start the current pattern over from the start
         * Will be called automatically at end of each pattern
        */
        inline void reset() {
            _curState = 0;
            _lastUpdate = millis();
        }

    private:
        inline bool shouldUpdate() const {
            return millis() - _lastUpdate >= currentPattern()->changeDelay;
        }

        CFastLED *_fastLed;
        Pattern *_patternArr;
        uint8_t _numPatterns;
        uint8_t _curPattern;
        uint16_t _curState;
        CRGB _curColor;
        uint32_t _lastUpdate;
};

#endif
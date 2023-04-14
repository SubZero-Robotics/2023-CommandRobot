#ifndef PATTERNS_H_
#define PATTERNS_H_

#include <Arduino.h>
#include <FastLED.h>

// C++11 constexpr is annoying
constexpr int32_t ceil(float num) {
    return (num == static_cast<float>(static_cast<int32_t>(num)))
               ? static_cast<int32_t>(num)
               : static_cast<int32_t>(num) + (num > 0 ? 1 : 0);
}

constexpr uint16_t ledNum = 150;

/**
 * Will be called after set delay has passed
 * @param state resets to 0 after current state >= numStates
 * @returns true if LEDs should show
 */
typedef bool (*ExecutePatternCallback)(CRGB *leds, CRGB color, uint16_t state,
                                       uint16_t ledCount);
constexpr uint8_t patternCount = 7;
constexpr uint8_t chaseLEDWidth = ceil((float)ledNum / 20);

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
    PatternRunner(CFastLED *fastLed, Pattern *patterns,
                  uint8_t numPatterns = patternCount)
        : _fastLed(fastLed),
          _patternArr(patterns),
          _numPatterns(numPatterns),
          _curPattern(0),
          _curState(0),
          _oneShot(false),
          _doneRunning(false),
          _curColor(CRGB::Black),
          _lastUpdate(0) {}

    /**
     * Call this every in every iteration of loop
     *
     * @param forceUpdate update even if not enough delay has passed
     */
    void update(bool forceUpdate = false);

    void setCurrentColor(CRGB color) {
        _curColor = color;
        reset();
    }

    /**
     * Set the current pattern
     * @returns true if successful
     */
    bool setCurrentPattern(PatternType, bool);
    bool setCurrentPattern(uint8_t, bool);

    /**
     * Get pointer to the current pattern
     */
    Pattern *currentPattern() const;

    inline bool patternDone() const { return _oneShot && _doneRunning; }

    static CRGB Wheel(uint8_t);

    /**
     * Start the current pattern over from the start
     * Will be called automatically at end of each pattern
     */
    inline void reset() {
        _curState = 0;
        _lastUpdate = millis();
        _doneRunning = false;
    }

   private:
    inline bool shouldUpdate() const {
        // Only update if enough delay has passed and pattern can be run again
        return (millis() - _lastUpdate >= currentPattern()->changeDelay) &&
               !(_oneShot && _doneRunning);
    }

    void incrementState(Pattern *);

    CFastLED *_fastLed;
    Pattern *_patternArr;
    uint8_t _numPatterns;
    uint8_t _curPattern;
    uint16_t _curState;
    bool _oneShot;
    bool _doneRunning;
    CRGB _curColor;
    uint32_t _lastUpdate;
};

#endif
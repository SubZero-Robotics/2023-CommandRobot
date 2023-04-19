#include "Patterns.h"

void PatternRunner::update(bool forceUpdate) {
    auto curPattern = currentPattern();

    if (forceUpdate) {
        incrementState(curPattern);

        return;
    }

    if (shouldUpdate()) {
        // If we're done, make sure to stop if one shot is set
        if (_curState >= curPattern->numStates) {
            if (_oneShot) {
                _doneRunning = true;
                return;
            } else {
                reset();
            }
        }

        incrementState(curPattern);
    }
}

void PatternRunner::incrementState(Pattern* curPattern) {
    if (curPattern->cb(_fastLed->leds(), _curColor, _curState,
                       _fastLed->size())) {
        _fastLed->show();
    }

    _curState++;
    _lastUpdate = millis();
}

bool PatternRunner::setCurrentPattern(uint8_t pattern, uint16_t delay, bool isOneShot = false) {
    if (pattern > patternCount - 1) {
        return false;
    }

    _curPattern = pattern;
    _oneShot = isOneShot;
    _delay = delay;
    reset();

    Serial.print("Set pattern to ");
    Serial.print(pattern);
    Serial.print(" | one-shot=");
    Serial.println(_oneShot);

    // Force the newly set pattern to run immediately
    update(true);

    return true;
}

bool PatternRunner::setCurrentPattern(PatternType type,
                                        uint16_t delay,
                                      bool isOneShot = false) {
    return setCurrentPattern((uint8_t)type, delay, isOneShot);
}

Pattern* PatternRunner::currentPattern() const {
    return &_patternArr[_curPattern];
}

CRGB PatternRunner::Wheel(uint8_t position) {
    if (position < 85) {
        return CRGB(position * 3, 255 - position * 3, 0);
    } else if (position < 170) {
        position -= 85;
        return CRGB(255 - position * 3, 0, position * 3);
    }
    return CRGB(0, position * 3, 255 - position * 3);
}
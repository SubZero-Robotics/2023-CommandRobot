#include "Patterns.h"

void PatternRunner::update() {
    auto curPattern = currentPattern();

    if (shouldUpdate()) {
        // If we're done, make sure to stop if one shot is set
        if (_curState >= curPattern->numStates) {
            /**if (_oneShot) {
                _doneRunning = true;
            } else {
                reset();
            }*/
            reset();
        }

        if (curPattern->cb(_fastLed->leds(), _curColor, _curState,
                           _fastLed->size())) {
            _fastLed->show();
        }

        _curState++;
        _lastUpdate = millis();
    }
}

bool PatternRunner::setCurrentPattern(uint8_t pattern, bool isOneShot = false) {
    if (pattern > patternCount - 1) {
        return false;
    }

    _curPattern = pattern;
    _oneShot = isOneShot;
    reset();

    Serial.print("Set pattern to ");
    Serial.println(pattern);

    return true;
}

bool PatternRunner::setCurrentPattern(PatternType type,
                                      bool isOneShot = false) {
    return setCurrentPattern((uint8_t)type, isOneShot);
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
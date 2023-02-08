#include "Patterns.h"

void PatternRunner::update() {
    auto curPattern = currentPattern();

    if (shouldUpdate()) {
        if (_curState >= curPattern->numStates) {
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

bool PatternRunner::setCurrentPattern(uint8_t pattern) {
    if (pattern > patternCount - 1) {
        return false;
    }

    _curPattern = pattern;
    reset();

    return true;
}

bool PatternRunner::setCurrentPattern(PatternType type) {
    return setCurrentPattern((uint8_t)type);
}

Pattern* PatternRunner::currentPattern() const {
    return &_patternArr[_curPattern];
}
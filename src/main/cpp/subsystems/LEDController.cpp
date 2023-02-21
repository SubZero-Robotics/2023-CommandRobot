#include "subsystems/LEDController.h"

LEDController(uint8_t slaveAddress, I2C::Port port = I2C::kOnboard) :
    _i2c = std::make_unique<I2C>(port, slaveAddress), _slaveAddress(slaveAddress),
    _lastCommand(LEDController::CommandType::Off),
    _lastPattern(LEDController::PatternType::None) {

}

bool LEDController::initialize() {
    return !_i2c->AddressOnly();
}

bool LEDController::setOn() {
    _lastCommand = LEDController::CommandType::On;
    uint8_t buf[1] = { (uint8_t)_lastCommand };
    return !_i2c->WriteBulk(buf, 1);
}

bool LEDController::setOff() {
    _lastCommand = LEDController::CommandType::Off;
    uint8_t buf[1] = { (uint8_t)_lastCommand };
    return !_i2c->WriteBulk(buf, 1);
}

bool LEDController::setPattern(LEDController::PatternType pattern, bool oneShot = false) {
    _lastCommand = LEDController::CommandType::Pattern;
    uint8_t buf[3] = {
        (uint8_t)_lastCommand,
        (uint8_t)pattern,
        oneShot
    };
    return !_i2c->WriteBulk(buf, 3);
}

bool LEDController::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    _lastCommand = LEDController::CommandType::ChangeColor;
    uint8_t buf[4] = {
        (uint8_t)_lastCommand,
        red,
        green,
        blue
    };
    return !_i2c->WriteBulk(buf, 4);
}

bool LEDController::setColor(uint32_t color) {
    uint8_t r = color >> 16 & 0xFF;
    uint8_t g = color >> 8 & 0xFF;
    uint8_t b = color & 0xFF;

    return setColor(r, g, b);
}

bool LEDController::getPatternDone() {
    _lastCommand = LEDController::CommandType::ReadPatternDone;
    uint8_t buf[1];
    _i2c->Read((uint8_t)_lastCommand, 1, buf);
    return buf[0];
}
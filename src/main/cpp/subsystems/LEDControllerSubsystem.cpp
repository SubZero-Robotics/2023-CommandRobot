#include "subsystems/LEDControllerSubsystem.h"

LEDControllerSubsystem::LEDControllerSubsystem(uint8_t slaveAddress,
                                               frc::I2C::Port port)
    : _i2c(std::make_unique<frc::I2C>(port, slaveAddress)),
      _slaveAddress(slaveAddress),
      _lastCommand(LEDControllerSubsystem::CommandType::Off),
      _lastPattern(LEDControllerSubsystem::PatternType::None) {
    setOn();
    Logging::logToStdOut("LEDS", "SET TO ON", Logging::Level::VERBOSE);
    setColor(Colors::Yellow);
    setPattern(LEDControllerSubsystem::PatternType::SetAll, true);
}

bool LEDControllerSubsystem::initialize() { return !_i2c->AddressOnly(); }

bool LEDControllerSubsystem::setOn() {
    _lastCommand = LEDControllerSubsystem::CommandType::On;
    uint8_t buf[1] = {(uint8_t)_lastCommand};
    return !_i2c->WriteBulk(buf, 1);
}

bool LEDControllerSubsystem::setOff() {
    _lastCommand = LEDControllerSubsystem::CommandType::Off;
    uint8_t buf[1] = {(uint8_t)_lastCommand};
    return !_i2c->WriteBulk(buf, 1);
}

bool LEDControllerSubsystem::setPattern(
    LEDControllerSubsystem::PatternType pattern, bool oneShot) {
    _lastCommand = LEDControllerSubsystem::CommandType::Pattern;
    uint8_t buf[3] = {(uint8_t)_lastCommand, (uint8_t)pattern, oneShot};
    return !_i2c->WriteBulk(buf, 3);
}

bool LEDControllerSubsystem::setColor(uint8_t red, uint8_t green,
                                      uint8_t blue) {
    // Logging::logToStdOut("LEDS", "SET COLOR START", Logging::Level::VERBOSE);
    _lastCommand = LEDControllerSubsystem::CommandType::ChangeColor;
    uint8_t buf[4] = {(uint8_t)_lastCommand, red, green, blue};
    _i2c->WriteBulk(buf, 4);
    // Logging::logToStdOut("LEDS", "SET COLOR END", Logging::Level::VERBOSE);
    return true;
}

bool LEDControllerSubsystem::setColor(uint32_t color) {
    uint8_t r = color >> 16 & 0xFF;
    uint8_t g = color >> 8 & 0xFF;
    uint8_t b = color & 0xFF;

    return setColor(r, g, b);
}

bool LEDControllerSubsystem::setColor(Colors color) {
    _currentColor = color;
    if (color == Colors::Yellow) {
        frc::SmartDashboard::PutString("LED State", "Yellow");
        return setColor(255, 255, 0);
    } else if (color == Colors::Purple) {
        frc::SmartDashboard::PutString("LED State", "Purple");
        return setColor(180, 0, 255);
    }

    return false;
}

frc2::CommandPtr LEDControllerSubsystem::SetMovementLED(uint32_t color,
                                                        PatternType pattern) {
    return frc2::InstantCommand(
               [this, color, pattern]() {
                   setOn();
                   setColor(color);
                   setPattern(pattern);
               },
               {this})
        .ToPtr();
}

frc2::CommandPtr LEDControllerSubsystem::DisplayCurrentColor() {
    return frc2::InstantCommand([this]() {
               setOn();
               setColor(_currentColor);
               setPattern(PatternType::SetAll);
           })
        .ToPtr();
}

bool LEDControllerSubsystem::getPatternDone() {
    _lastCommand = LEDControllerSubsystem::CommandType::ReadPatternDone;
    uint8_t buf[1];
    _i2c->Read((uint8_t)_lastCommand, 1, buf);
    return buf[0];
}
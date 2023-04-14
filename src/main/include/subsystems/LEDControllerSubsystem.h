#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <frc/I2C.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SubsystemBase.h>

#include <memory>

#include "Constants.h"
#include "utils/Logging.h"

class LEDControllerSubsystem : public frc2::SubsystemBase {
   public:
    enum class CommandType {
        On = 0,
        Off = 1,
        Pattern = 2,
        ChangeColor = 3,
        ReadPatternDone = 4
    };

    enum class PatternType {
        None = 0,
        SetAll = 1,
        Blink = 2,
        RGBFade = 3,
        HackerMode = 4,
        Chase = 5,
        Wipe = 6
    };

    enum class Colors { Yellow, Purple };

    LEDControllerSubsystem(uint8_t slaveAddress,
                           frc::I2C::Port port = frc::I2C::kOnboard);

    /**
     * @brief Start communication with the LED controller
     *
     * @return true if successful init
     */
    bool initialize();

    /**
     * @brief Get the last command sent
     *
     * @return CommandType
     */
    inline CommandType lastCommand() const { return _lastCommand; }

    inline PatternType lastPattern() const { return _lastPattern; }

    /**
     * @brief Send the ON command
     *
     */
    bool setOn();

    /**
     * @brief Send the OFF command
     *
     */
    bool setOff();

    /**
     * @brief Send the PATTERN command
     *
     * @param pattern
     * @param oneShot Only run the pattern once
     */
    bool setPattern(PatternType pattern, bool oneShot = false);

    /**
     * @brief Send the COLOR command
     *
     */
    bool setColor(uint8_t red, uint8_t green, uint8_t blue);
    /**
     * @brief Send the COLOR command
     *
     * @param color Color data in the form of 0x00RRGGBB
     */
    bool setColor(uint32_t color);

    bool setColor(Colors color);

    frc2::CommandPtr SetMovementLED(uint32_t color, PatternType pattern);

    frc2::CommandPtr DisplayCurrentColor();

    /**
     * @brief Send the READPATTERNDONE command
     *
     * @return true if pattern is done
     */
    bool getPatternDone();

    inline Colors getCurrentColor() const { return _currentColor; }

   private:
    std::unique_ptr<frc::I2C> _i2c;
    uint8_t _slaveAddress;
    CommandType _lastCommand;
    PatternType _lastPattern;
    Colors _currentColor;
};

#endif
#ifndef BASE_SINGLE_AXIS_SUBSYSTEM_H
#define BASE_SINGLE_AXIS_SUBSYSTEM_H

#include <frc/DigitalInput.h>
#include <frc/DutyCycleEncoder.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxPIDController.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/length.h>
#include <units/velocity.h>

#include <memory>

#include "Constants.h"
#include "subsystems/ISingleAxisSubsystem.h"
#include "utils/Logging.h"

/**
 * @brief A base class for a single-axis subsystem
 *
 * @example BaseSingleAxisSubsystem<rev::CANSparkMax,
 units::meters>::SingleAxisConfig config = {
 BaseSingleAxisSubsystem<rev::CANSparkMax, units::meters>::AxisType::Linear, //
 type frc::ProfilePIDController(1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::meters>::Constraints(1.75_mps,
 0.75_mps_sq), 20_ms   // kDt (s)
        ),   // PID
        0,      // min distance
        200,    // max distance
        30,     // distance per revolution in linear units
        1,      // motor direction
        0,      // min limit switch port
        1,      // max limit switch port
        2,      // encoder port
        0.33    // default movement speed of 33%
    };

    rev::CANSparkMax m_leadRotationMotor{
        CANSparkMaxConstants::kLeadRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};

    BaseSingleAxisSubsystem<rev::CANSparkMax, units::meters> singleAxis =
 BaseSingleAxisSubsystem<rev::CANSparkMax, units::meters>(config,
 m_leadRotationMotor);
 *
 * @tparam Motor Any motor that supports Set(percent)
 * @tparam Unit Position unit (units::meters, etc.)
 */
template <typename Motor, typename Encoder, typename Unit, typename Unit_t>
class BaseSingleAxisSubsystem : public ISingleAxisSubsystem,
                                public frc2::SubsystemBase {
   public:
    enum ConfigConstants {
        MOTOR_DIRECTION_NORMAL = 1,
        MOTOR_DIRECTION_REVERSED = -1,
        UNUSED_DIO_PORT = -1
    };

    enum class AxisType { Rotational, Linear };

    /**
     * @brief Configuration for a single axis of absolute movement
     *
     * @param type Rotational or Linear direction
     * @param pid ProfiledPIDController for moving the axis along a profile
     * @param minDistance Minimum distance in your choice of linear or
     * rotational units
     * @param maxDistance Maximum distance in your choice of linear or
     * rotational units
     * @param distancePerRevolution Distance the axis moves per revolution of
     * the motor
     * @param motorMultiplier Set to 1.0 or -1.0 to reverse motor direction.
     * Negative is always decreasing distance
     */
    struct SingleAxisConfig {
        AxisType type;
        frc::ProfiledPIDController<Unit> pid;
        double minDistance;
        double maxDistance;
        double distancePerRevolution;
        double stepSize;
        double motorMultiplier = 1.0;
        int minLimitSwitchPort = UNUSED_DIO_PORT;
        int maxLimitSwitchPort = UNUSED_DIO_PORT;
        double defaultMovementSpeed = 0.2;
    };

    BaseSingleAxisSubsystem(SingleAxisConfig cfg, Motor &motor,
                            Encoder &encoder, frc::DigitalInput *minSwitch,
                            frc::DigitalInput *maxSwitch, std::string prefix,
                            bool log = false)
        : _motor(motor),
          _enc(encoder),
          _config(cfg),
          _controller(cfg.pid),
          _isHoming(false),
          _isMovingToPosition(false),
          _targetPosition(0),
          _log(log),
          _prefix(prefix),
          _minLimitSwitch(minSwitch),
          _maxLimitSwitch(maxSwitch) {
        _config.defaultMovementSpeed =
            std::clamp(_config.defaultMovementSpeed, -1.0, 1.0);
    }

    /**
     * @brief Run motor at the specified speed.
     * ! Make sure to use _config.motorMultiplier to invert if needed!
     * This method will also prevent movement in certain directions if at a
     * limit. Use RunMotorExternal if moving the motor from an external command
     *
     * @param speed Percentage speed
     */
    void RunMotorSpeed(double speed, bool ignoreEncoder = false) override {
        if (_log)
            Logging::logToStdOut(
                _prefix, "MUL IS " + std::to_string(_config.motorMultiplier),
                Logging::Level::VERBOSE);
        speed *= _config.motorMultiplier;
        speed = std::clamp(speed, -1.0, 1.0);
        if (_log)
            Logging::logToStdOut(_prefix, "SPEED IS " + std::to_string(speed),
                                 Logging::Level::VERBOSE);

        bool homeState = ignoreEncoder ? AtLimitSwitchHome() : AtHome();
        if (homeState) {
            if (_log)
                Logging::logToStdOut(_prefix, "AT HOME", Logging::Level::INFO);
            if (speed > 0) {
                if (_log)
                    Logging::logToStdOut(
                        _prefix, "SETTING SPEED TO: " + std::to_string(speed),
                        Logging::Level::VERBOSE);
                _motor.Set(speed);
                return;
            }

            if (_log)
                Logging::logToStdOut(
                    _prefix, "NOT MOVING; AT HOME" + std::to_string(speed),
                    Logging::Level::VERBOSE);

            _motor.Set(0);
            return;
        }

        if (AtMax()) {
            if (_log)
                Logging::logToStdOut(_prefix, "AT MAX", Logging::Level::INFO);
            if (speed < 0) {
                if (_log)
                    Logging::logToStdOut(
                        _prefix, "SETTING SPEED TO: " + std::to_string(speed),
                        Logging::Level::VERBOSE);
                _motor.Set(speed);
                return;
            }

            if (_log)
                Logging::logToStdOut(
                    _prefix, "NOT MOVING; AT MAX " + std::to_string(speed),
                    Logging::Level::VERBOSE);
            _motor.Set(0);
            return;
        }

        if (_log)
            Logging::logToStdOut(_prefix,
                                 "SETTING SPEED TO: " + std::to_string(speed),
                                 Logging::Level::VERBOSE);
        _motor.Set(speed);
    }

    void RunMotorSpeedDefault(bool invertDirection = false) override {
        RunMotorSpeed(invertDirection ? -_config.defaultMovementSpeed
                                      : _config.defaultMovementSpeed);
    }

    /**
     * @brief Call this one from a joystick-bound command to override current
     * movement
     *
     * @param speed Percentage speed
     */
    void RunMotorExternal(double speed) override {
        // TODO: constant
        if (abs(speed) <= 0.05) {
            if (_config.type == AxisType::Rotational)
                _motor.Set(ArmConstants::kAntiGravityPercentage);
        }

        if (_isMovingToPosition) {
            StopMovement();
        }

        RunMotorSpeed(speed);
    }

    void JoystickMoveStep(double rotation) override {
        auto steps = rotation * _config.stepSize;
        auto newTarget = IncrementTargetPosition(steps);
        MoveToPosition(newTarget);
    }

    double IncrementTargetPosition(double steps) override {
        return std::clamp(_targetPosition + steps, _config.minDistance,
                          _config.maxDistance);
    }

    void UpdateMovement() override {
        if (_isMovingToPosition) {
            if (_log)
                Logging::logToStdOut(
                    _prefix,
                    "Target Position: " +
                        std::to_string(Unit_t(_targetPosition).value()),
                    Logging::Level::INFO);
            auto res = _controller.Calculate(Unit_t(GetCurrentPosition()),
                                             Unit_t(_targetPosition));
            auto clampedRes = std::clamp(res, -1.0, 1.0);
            if (_log)
                Logging::logToStdOut(_prefix,
                                     "Clamped Res: " + std::to_string(res),
                                     Logging::Level::INFO);
            Logging::logToSmartDashboard(_prefix + " TargetPosition",
                                         std::to_string(_targetPosition),
                                         Logging::Level::INFO);
            RunMotorSpeed(clampedRes);
        }
    }

    virtual void ResetEncoder() = 0;

    virtual double GetCurrentPosition() = 0;

    bool AtHome() override {
        if (_minLimitSwitch) {
            if (AtLimitSwitchHome()) {
                ResetEncoder();
                if (_log)
                    Logging::logToStdOut(_prefix, "AT HOME SWITCH",
                                         Logging::Level::INFO);
                return true;
            }
        }

        // TODO: Constant wrap-around value
        if (GetCurrentPosition() <= _config.minDistance ||
            GetCurrentPosition() >= 350.0) {
            if (_log)
                Logging::logToStdOut(_prefix, "AT HOME ENC",
                                     Logging::Level::INFO);
            return true;
        }

        return false;
    }

    inline bool AtMax() override {
        if (_maxLimitSwitch) {
            if (AtLimitSwitchMax()) {
                if (_log)
                    Logging::logToStdOut(_prefix, "AT MAX SWITCH",
                                         Logging::Level::INFO);
                return true;
            }
        }

        if (GetCurrentPosition() >= _config.maxDistance &&
            GetCurrentPosition() < 350) {
            if (_log)
                Logging::logToStdOut(_prefix, "AT MAX ENC",
                                     Logging::Level::INFO);

            return true;
        }

        return false;
    }

    inline bool AtLimitSwitchHome() override {
        if (_minLimitSwitch) {
            auto state = !_minLimitSwitch->Get();
            if (_log)
                Logging::logToStdOut(
                    _prefix, "MIN LIMIT SWITCH: " + std::to_string(state),
                    Logging::Level::VERBOSE);
            return state;
        }

        return false;
    }

    inline bool AtLimitSwitchMax() override {
        if (_maxLimitSwitch) {
            auto state = !_maxLimitSwitch->Get();
            if (_log)
                Logging::logToStdOut(
                    _prefix, "MAX LIMIT SWITCH: " + std::to_string(state),
                    Logging::Level::VERBOSE);
            return state;
        }

        return false;
    }

    void MoveToPosition(double position) override {
        if (_log)
            Logging::logToStdOut(_prefix,
                                 "Moving to " + std::to_string(position),
                                 Logging::Level::INFO);
        _isMovingToPosition = true;
        _targetPosition = position;
    }

    void Home() override {
        if (_log)
            Logging::logToStdOut(_prefix, "Set homing to true",
                                 Logging::Level::INFO);
        StopMovement();
        _isHoming = true;
    }

    inline bool GetIsMovingToPosition() override { return _isMovingToPosition; }

    inline void StopMovement() override {
        if (_log)
            Logging::logToStdOut(_prefix, "Movement stopped",
                                 Logging::Level::INFO);
        _isHoming = false;
        _isMovingToPosition = false;
        _motor.Set(0);
    }

    frc2::CommandPtr GetHomeCommand() override {
        return frc2::FunctionalCommand(
                   [this] { Home(); },
                   // Ignore the home encoder value since it starts at 0
                   [this] {
                       RunMotorSpeed(-_config.defaultMovementSpeed, true);
                   },
                   [this](bool interrupted) {
                       StopMovement();
                       ResetEncoder();
                   },
                   // Finish once limit switch is hit
                   [this] { return AtLimitSwitchHome(); }, {this})
            .ToPtr();
    }

    void Periodic() override {
        // auto res = frc::SmartDashboard::GetNumber(_prefix + " Position Set",
        // 0); if (_log)
        //     Logging::logToStdOut(_prefix, "Shuffleboard Position" +
        //     std::to_string(res),
        //                          Logging::Level::INFO);
        // MoveToPosition(res);
        UpdateMovement();
    }

    static inline bool IsValidPort(int port) { return port >= 0 && port < 10; }

   protected:
    Motor &_motor;
    Encoder &_enc;
    SingleAxisConfig _config;
    frc::ProfiledPIDController<Unit> _controller;
    bool _isHoming = false;
    bool _isMovingToPosition = false;
    double _targetPosition = 0.0;
    bool _log;
    std::string _prefix;

   private:
    frc::DigitalInput *_minLimitSwitch = nullptr;
    frc::DigitalInput *_maxLimitSwitch = nullptr;
};

#endif
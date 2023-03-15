#ifndef BASE_SINGLE_AXIS_SUBSYSTEM_H
#define BASE_SINGLE_AXIS_SUBSYSTEM_H

#include <frc/DigitalInput.h>
#include <frc/DutyCycleEncoder.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/SubsystemBase.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/length.h>
#include <units/velocity.h>

#include <memory>

#include "Constants.h"
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
class BaseSingleAxisSubsystem : public frc2::SubsystemBase {
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
    void RunMotorSpeed(double speed) {
        speed *= _config.motorMultiplier;
        speed = std::clamp(speed, -1.0, 1.0);
        if (_log)
            Logging::logToStdOut(_prefix, "SPEED IS " + std::to_string(speed),
                                 Logging::Level::VERBOSE);
        if (AtHome()) {
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

            _motor.Set(0);
            return;
        } else if (AtMax()) {
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

            _motor.Set(0);
            return;
        } else {
            if (_log)
                Logging::logToStdOut(
                    _prefix, "SETTING SPEED TO: " + std::to_string(speed),
                    Logging::Level::VERBOSE);
            _motor.Set(speed);
        }
    }

    void RunMotorSpeedDefault(bool invertDirection = false) {
        RunMotorSpeed(invertDirection ? -_config.defaultMovementSpeed
                                      : _config.defaultMovementSpeed);
    }

    /**
     * @brief Call this one from a joystick-bound command to override current
     * movement
     *
     * @param speed Percentage speed
     */
    void RunMotorExternal(double speed) {
        if (_isMovingToPosition) {
            StopMovement();
        }

        RunMotorSpeed(speed);
    }

    void UpdateMovement() {
        if (_isMovingToPosition) {
            double res = std::clamp(
                _controller.Calculate(Unit_t(GetCurrentPosition()),
                                      Unit_t(_targetPosition)),
                -_config.defaultMovementSpeed, _config.defaultMovementSpeed);
            if (_log)
                Logging::logToStdOut(_prefix,
                                     "PID returned " + std::to_string(res),
                                     Logging::Level::INFO);
            if (!_controller.AtGoal()) {
                RunMotorSpeed(res);
            } else {
                _isMovingToPosition = false;
            }
        }
    }

    virtual void ResetEncoder() = 0;

    virtual double GetCurrentPosition() = 0;

    bool AtHome() {
        if (_minLimitSwitch) {
            if (AtLimitSwitchHome()) {
                ResetEncoder();
                if (_log)
                    Logging::logToStdOut(_prefix, "AT HOME SWITCH",
                                         Logging::Level::INFO);
                return true;
            }
        }

        if (GetCurrentPosition() <= _config.minDistance) {
            ResetEncoder();
            if (_log)
                Logging::logToStdOut(_prefix, "AT HOME ENC",
                                     Logging::Level::INFO);
            return true;
        }

        return false;
    }

    inline bool AtMax() {
        if (_maxLimitSwitch) {
            if (_log)
                Logging::logToStdOut(_prefix, "in at max lim switch",
                                     Logging::Level::INFO);
            if (AtLimitSwitchMax()) {
                return true;
            }
        }

        return GetCurrentPosition() >= _config.maxDistance;
    }

    inline bool AtLimitSwitchHome() const {
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

    inline bool AtLimitSwitchMax() const {
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

    void MoveToPosition(double position) {
        if (_log)
            Logging::logToStdOut(_prefix,
                                 "Moving to " + std::to_string(position),
                                 Logging::Level::INFO);
        _isMovingToPosition = true;
        _targetPosition = position;
        _controller.SetGoal(position);
    }

    void Home() {
        if (_log)
            Logging::logToStdOut(_prefix, "Set homing to true",
                                 Logging::Level::INFO);
        _isHoming = true;
    }

    inline bool GetIsMovingToPosition() const { return _isMovingToPosition; }

    inline void StopMovement() {
        if (_log)
            Logging::logToStdOut(_prefix, "Movement stopped",
                                 Logging::Level::INFO);
        _isHoming = false;
        _isMovingToPosition = false;
        _motor.Set(0);
    }

    frc2::CommandPtr GetHomeCommand() {
        return frc2::FunctionalCommand(
                   [this] { Home(); },
                   [this] { RunMotorSpeed(-_config.defaultMovementSpeed); },
                   [this](bool interrupted) {
                       StopMovement();
                       ResetEncoder();
                   },
                   [this] { return AtHome(); }, {this})
            .ToPtr();
    }

    void Periodic() override { UpdateMovement(); }

    static inline bool IsValidPort(int port) { return port >= 0 && port < 10; }

   protected:
    Motor &_motor;
    Encoder &_enc;
    SingleAxisConfig _config;
    frc::ProfiledPIDController<Unit> _controller;
    bool _isHoming;
    bool _isMovingToPosition;
    double _targetPosition;
    bool _log;
    std::string _prefix;

   private:
    frc::DigitalInput *_minLimitSwitch = nullptr;
    frc::DigitalInput *_maxLimitSwitch = nullptr;
};

#endif
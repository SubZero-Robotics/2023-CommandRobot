#ifndef BASE_SINGLE_AXIS_SUBSYSTEM_H
#define BASE_SINGLE_AXIS_SUBSYSTEM_H

#include <memory>
#include <frc/DigitalInput.h>
#include <frc/DutyCycleEncoder.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <frc/controller/PIDController.h>

#include "constants.h"

/**
 * @brief A base class for a single-axis subsystem
 * 
 * @example BaseSingleAxisSubsystem<rev::CANSparkMax>::SingleAxisConfig config = {
        BaseSingleAxisSubsystem<rev::CANSparkMax>::AxisType::Linear,    // type
        frc2::PIDController(1, 2, 3),   // PID
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

    BaseSingleAxisSubsystem<rev::CANSparkMax> singleAxis = BaseSingleAxisSubsystem<rev::CANSparkMax>(config, m_leadRotationMotor);
 * 
 * @tparam Motor Any motor that supports Set(percent)
 */
template <typename Motor>
class BaseSingleAxisSubsystem : public frc2::SubsystemBase {
    public:
        enum ConfigConstants {
            MOTOR_DIRECTION_NORMAL = 1,
            MOTOR_DIRECTION_REVERSED = -1,
            UNUSED_DIO_PORT = -1
        };

        enum class AxisType {
            Rotational,
            Linear
        };

        /**
         * @brief Configuration for a single axis of absolute movement
         * 
         * @param type Rotational or Linear direction
         * @param pid PIDController for moving the axis
         * @param minDistance Minimum distance in your choice of linear or rotational units
         * @param maxDistance Maximum distance in your choice of linear or rotational units
         * @param distancePerRevolution Distance the axis moves per revolution of the motor
         * @param motorDirection Set to 1 or -1 to reverse motor direction. Negative is always less distance
         */
        struct SingleAxisConfig {
            AxisType type;
            frc2::PIDController pid;
            double minDistance;
            double maxDistance;
            double distancePerRevolution;
            ConfigConstants motorDirection = MOTOR_DIRECTION_NORMAL;
            int minLimitSwitchPort = (int)UNUSED_DIO_PORT;
            int maxLimitSwitchPort = (int)UNUSED_DIO_PORT;
            int absoluteEncoderPort = (int)UNUSED_DIO_PORT;
            double defaultMovementSpeed = 0.2;
        };

        BaseSingleAxisSubsystem(SingleAxisConfig cfg, Motor &&motor) :
            _motor(std::move(motor)), _config(cfg), _pid(cfg.pid), _isHoming(false), _isMovingToPosition(false),
            _targetPosition(0) {

            if (IsValidPort(_config.absoluteEncoderPort)) {
                _encoder = std::make_unique<frc::DutyCycleEncoder>(_config.absoluteEncoderPort);
                _encoder->SetDistancePerRotation(_config.distancePerRevolution);
            }

            if (IsValidPort(_config.minLimitSwitchPort)) {
                _minLimitSwitch = std::make_unique<frc::DigitalInput>(_config.minLimitSwitchPort);
            }

            if (IsValidPort(_config.maxLimitSwitchPort)) {
                _maxLimitSwitch = std::make_unique<frc::DigitalInput>(_config.maxLimitSwitchPort);
            }

            _config.defaultMovementSpeed = std::clamp(_config.defaultMovementSpeed, -1.0, 1.0);
        }

        /**
         * @brief Run motor at the specified speed.
         * ! Make sure to use _config.motorDirection to invert if needed!
         * This method will also prevent movement in certain directions if at a limit
         * 
         * @param speed Percentage speed
         */
        void RunMotorSpeed(double speed) const {
            speed *= (int)_config.motorDirection;

            if (AtHome()) {
                if (speed > 0) {
                    _motor.Set(speed);
                    return;
                }

                _motor.Set(0);
                return;
            } else if (AtMax()) {
                if (speed < 0) {
                    _motor.Set(speed);
                    return;
                }

                _motor.Set(0);
                return;
            } else {
                _motor.Set(speed);
            }
        }

        void UpdateMovement() {
            if (_isMovingToPosition) {
                auto res = std::clamp(_pid.Calculate(GetCurrentPosition(), _targetPosition),
                    -_config.defaultMovementSpeed, _config.defaultMovementSpeed);
                if (!_pid.AtSetpoint()) {
                    RunMotorSpeed(res);
                } else {
                    _isMovingToPosition = false;
                }
            }
        }

        inline void ResetEncoder() const {
            _encoder->Reset();
        }

        inline double GetCurrentPosition() const {
            if (_encoder) {
                return _encoder->GetDistance();
            }
        }

        bool AtHome() const {
            if (_minLimitSwitch) {
                if (AtLimitSwitchHome()) {
                    ResetEncoder();
                    return true;
                }
            }
            
            if (_encoder) {
                if (GetCurrentPosition() <= _config.minDistance) {
                    ResetEncoder();
                    return true;
                }
            }

            return false;
        }

        inline bool AtMax() const {
            if (_maxLimitSwitch) {
                return AtLimitSwitchMax();
            }

            if (_encoder) {
                return GetCurrentPosition() >= _config.maxDistance;
            }

            return false;
        }

        inline bool AtLimitSwitchHome() const {
            if (_minLimitSwitch) {
                return !_minLimitSwitch->Get();
            }

            return false;
        }

        inline bool AtLimitSwitchMax() const {
            if (_maxLimitSwitch) {
                return !_maxLimitSwitch->Get();
            }

            return false;
        }

        void MoveToPosition(double position) {
            _isMovingToPosition = true;
            _targetPosition = position;
        }

        void Home() {
            _isHoming = true;
        }

        inline bool GetIsMovingToPosition() const {
            return _isMovingToPosition;
        }

        inline void StopMovement() {
            _isHoming = false;
            _isMovingToPosition = false;
            _motor->Set(0);
        }

        frc2::CommandPtr GetHomeCommand() {
            return frc2::FunctionalCommand(
                [this] { Home(); },
                [this] { RunMotorSpeed(-_config.defaultMovementSpeed); },
                [this] (bool interrupted) {
                    RunMotorSpeed(0);
                    ResetEncoder();
                    _isHoming = false;
                },
                [this] { AtHome(); },
                { this }
            ).ToPtr();
        }

        void Periodic() override {
            UpdateMovement();
        }

        static inline bool IsValidPort(int port) {
            return port >= 0 && port < 10;
        }

    protected:
        Motor _motor;
        SingleAxisConfig _config;
        std::unique_ptr<frc::DutyCycleEncoder> _encoder;
        frc2::PIDController _pid;
        bool _isHoming;
        bool _isMovingToPosition;
        double _targetPosition;

    private:
        std::unique_ptr<frc::DigitalInput> _minLimitSwitch;
        std::unique_ptr<frc::DigitalInput> _maxLimitSwitch;
};

#endif
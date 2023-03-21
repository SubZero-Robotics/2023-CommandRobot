#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"

class RotateArmSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxAbsoluteEncoder,
                                     units::degree, units::degree_t> {
   public:
    RotateArmSubsystem()
        : BaseSingleAxisSubsystem(m_config, m_leadRotationMotor, m_enc, &min,
                                  &max, "ARM", "\033[95;40;4m") {
        m_followRotationMotor.Follow(m_leadRotationMotor);
        // https://docs.wpilib.org/en/stable/docs/software/advanced-controls/introduction/tuning-vertical-arm.html
        _config = m_config;
        _controller = m_config.pid;
        _controller.SetTolerance(10, 15);
        m_enc.SetPositionConversionFactor(1);
    }

    // Rotate arm has zero offset set in SparkMax
    void ResetEncoder() override {}

    double GetCurrentPosition() override {
        // "Home" is at 60 deg relative to ground
        auto position = m_enc.GetPosition() * _config.distancePerRevolution +
                        _config.minDistance;

        if (position >= 350) position = 0;

        Logging::logToSmartDashboard("ArmPosition",
                                     std::to_string(position) + " deg",
                                     Logging::Level::INFO);

        if (_log)
            Logging::logToStdOut(_prefix,
                                 std::to_string(position) + "/" +
                                     std::to_string(_config.maxDistance) +
                                     " deg",
                                 Logging::Level::INFO);

        return position;
    }

    void UpdateMovement() override {
        if (_isMovingToPosition) {
            if (_log)
                Logging::logToStdOut(
                    _prefix,
                    "Target Position: " +
                        std::to_string(_targetPosition) +
                        std::string(_config.type == AxisType::Linear ? " in"
                                                                     : " deg"),
                    Logging::Level::INFO, _ansiPrefixModifiers);

            auto res = _controller.Calculate(GetCurrentPosition(),
                                             _targetPosition) * -1;
            auto clampedRes = std::clamp(res, -1.0, 1.0) * 0.65;
            if (_log)
                Logging::logToStdOut(
                    _prefix, "Clamped Res: " + std::to_string(clampedRes),
                    Logging::Level::INFO, _ansiPrefixModifiers);
            Logging::logToSmartDashboard(_prefix + " TargetPos",
                                         std::to_string(_targetPosition),
                                         Logging::Level::INFO);

            if (_controller.AtSetpoint()) {
                Logging::logToStdOut(_prefix, "REACHED GOAL",
                                     Logging::Level::INFO,
                                     _ansiPrefixModifiers);
                StopMovement();
                return;
            }

            RunMotorSpeed(clampedRes);
        }
    }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    rev::CANSparkMax m_leadRotationMotor{
        CANSparkMaxConstants::kArmRotationLeadMotorID,
        rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_followRotationMotor{
        CANSparkMaxConstants::kArmRotationFollowMotorID,
        rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxAbsoluteEncoder m_enc =
        m_followRotationMotor.GetAbsoluteEncoder(
            rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle);

    SingleAxisConfig m_config = {
        .type = BaseSingleAxisSubsystem::AxisType::Rotational,
        .pid = frc2::PIDController(ArmConstants::kArmRotationSetP,
                                   ArmConstants::kArmRotationSetI,
                                   ArmConstants::kArmRotationSetD),
        .minDistance = ArmConstants::kRotationHomeDegree,
        .maxDistance = ArmConstants::kRotationMaxDegree,
        .distancePerRevolution = 360.0,
        .stepSize = ArmConstants::kArmStepSize,
        .motorMultiplier = 1.0,
        .minLimitSwitchPort = ArmConstants::kRotationLimitSwitchHomePort,
        .maxLimitSwitchPort = ArmConstants::kRotationLimitSwitchMaxPort,
        .defaultMovementSpeed = ArmConstants::kRotationHomingSpeed};

    frc::DigitalInput min{ArmConstants::kRotationLimitSwitchHomePort};
    frc::DigitalInput max{ArmConstants::kRotationLimitSwitchMaxPort};
};
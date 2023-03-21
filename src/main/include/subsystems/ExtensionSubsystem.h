#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"
#include "utils/Logging.h"

class ExtensionSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxRelativeEncoder, units::inch,
                                     units::inch_t> {
   public:
    ExtensionSubsystem()
        : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, &min,
                                  nullptr, "EXTEND", "\033[96;40;4m") {
        m_extensionMotor.SetInverted(true);
        _config = m_config;
        _controller = m_config.pid;
        _config.distancePerRevolution = ArmConstants::kInPerRotation;
        m_encoder.SetPositionConversionFactor(1);
    }

    void ResetEncoder() override {
        if (_log)
            Logging::logToStdOut(_prefix, "RESET POSITION",
                                 Logging::Level::INFO);
        m_encoder.SetPosition(0);
    }

    double GetCurrentPosition() override {
        auto position = m_encoder.GetPosition() * _config.distancePerRevolution;

        Logging::logToSmartDashboard("ExtensionPosition",
                                     std::to_string(position) + " in",
                                     Logging::Level::INFO);

        if (_log)
            Logging::logToStdOut(_prefix,
                                 std::to_string(position) + "/" +
                                     std::to_string(_config.maxDistance) +
                                     " in",
                                 Logging::Level::INFO);

        return position;
    }

    void UpdateMovement() override {
        if (_isMovingToPosition) {
            if (_log)
                Logging::logToStdOut(
                    _prefix,
                    "Target Position: " + std::to_string(_targetPosition) +
                        std::string(_config.type == AxisType::Linear ? " in"
                                                                     : " deg"),
                    Logging::Level::INFO, _ansiPrefixModifiers);

            // TODO: extract multipliers to constants and pass through the
            // config
            auto res =
                _controller.Calculate(GetCurrentPosition(), _targetPosition) *
                2.0;
            auto clampedRes = std::clamp(res, -1.0, 1.0);
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
    rev::CANSparkMax m_extensionMotor{CANSparkMaxConstants::kExtensionMotorID,
                                      rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor,
        ArmConstants::kTicksPerMotorRotation);

    SingleAxisConfig m_config = {
        .type = BaseSingleAxisSubsystem::AxisType::Linear,
        .pid = frc2::PIDController(ArmConstants::kExtenderSetP,
                                   ArmConstants::kExtenderSetI,
                                   ArmConstants::kExtenderSetD),
        .minDistance = 0,
        .maxDistance = ArmConstants::kMaxArmDistance,
        .distancePerRevolution = ArmConstants::kInPerRotation,
        .stepSize = ArmConstants::kExtenderStepSize,
        .motorMultiplier = .5,
        .minLimitSwitchPort = ArmConstants::kExtenderLimitSwitchPort,
        .maxLimitSwitchPort = BaseSingleAxisSubsystem::UNUSED_DIO_PORT,
        .defaultMovementSpeed = ArmConstants::kExtenderHomingSpeed};

    frc::DigitalInput min{ArmConstants::kExtenderLimitSwitchPort};
};
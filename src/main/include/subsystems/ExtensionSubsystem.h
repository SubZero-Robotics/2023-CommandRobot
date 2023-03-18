#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"
#include "utils/Logging.h"

class ExtensionSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxRelativeEncoder, units::meter,
                                     units::meter_t> {
   public:
    ExtensionSubsystem()
        : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, &min,
                                  nullptr, "EXTEND", true) {
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

   private:
    rev::CANSparkMax m_extensionMotor{CANSparkMaxConstants::kExtensionMotorID,
                                      rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor,
        ArmConstants::kTicksPerMotorRotation);

    SingleAxisConfig m_config = {
        .type = BaseSingleAxisSubsystem::AxisType::Linear,
        .pid = frc::ProfiledPIDController<units::meter>(
            ArmConstants::kExtenderSetP, ArmConstants::kExtenderSetI,
            ArmConstants::kExtenderSetD,
            frc::TrapezoidProfile<units::meter>::Constraints(1.75_mps,
                                                             0.75_mps_sq)),
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
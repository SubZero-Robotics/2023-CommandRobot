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
        : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, m_pid,
                                  &min, nullptr, "EXTEND") {
        m_pid.SetFeedbackDevice(m_encoder);
        // https://docs.wpilib.org/en/stable/docs/software/advanced-controls/introduction/tuning-vertical-arm.html
        m_pid.SetOutputRange(-1.0, 1.0);
        // TODO
        m_pid.SetP(ArmConstants::kExtenderSetP);
        m_pid.SetI(ArmConstants::kExtenderSetI);
        m_pid.SetD(ArmConstants::kExtenderSetD);
        m_pid.SetIZone(ArmConstants::kExtenderSetIZone);
        m_pid.SetFF(ArmConstants::kExtenderSetFF);
        _config = m_config;
        m_encoder.SetPositionConversionFactor(_config.distancePerRevolution);
    }

    void ResetEncoder() override {
        if (_log)
            Logging::logToStdOut(_prefix, "RESET POSITION",
                                 Logging::Level::INFO);
        m_encoder.SetPosition(0);
    }

    double GetCurrentPosition() override {
        auto position = m_encoder.GetPosition();

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

    rev::SparkMaxPIDController m_pid = m_extensionMotor.GetPIDController();

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor,
        ArmConstants::kTicksPerMotorRotation);

    SingleAxisConfig m_config = {
        .type = BaseSingleAxisSubsystem::AxisType::Linear,
        .pid = frc::ProfiledPIDController<units::meter>(
            1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::meter>::Constraints(1.75_mps,
                                                             0.75_mps_sq)),
        .minDistance = 0,
        .maxDistance = ArmConstants::kMaxArmDistance,
        .distancePerRevolution = ArmConstants::kInPerRotation,
        .motorMultiplier = 1.0,
        .minLimitSwitchPort = ArmConstants::kExtenderLimitSwitchPort,
        .maxLimitSwitchPort = BaseSingleAxisSubsystem::UNUSED_DIO_PORT,
        .defaultMovementSpeed = ArmConstants::kExtenderHomingSpeed};

    frc::DigitalInput min{ArmConstants::kExtenderLimitSwitchPort};
};
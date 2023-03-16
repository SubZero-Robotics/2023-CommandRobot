#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"

class WristSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxAbsoluteEncoder,
                                     units::degree, units::degree_t> {
   public:
    WristSubsystem()
        : BaseSingleAxisSubsystem(m_config, m_wristMotor, m_encoder, m_pid,
                                  &min, nullptr, "WRIST", true) {
        m_pid.SetFeedbackDevice(m_encoder);
        // TODO
        m_pid.SetP(ArmConstants::kWristSetP);
        m_pid.SetI(ArmConstants::kWristSetI);
        m_pid.SetD(ArmConstants::kWristSetD);
        m_pid.SetIZone(ArmConstants::kWristSetIZone);
        m_pid.SetFF(ArmConstants::kWristSetFF);
        _config = m_config;
        m_encoder.SetPositionConversionFactor(_config.distancePerRevolution);
    }

    // Wrist has zero offset set in SparkMax
    void ResetEncoder() override {}

    double GetCurrentPosition() override {
        auto position = m_encoder.GetPosition();

        Logging::logToSmartDashboard("WristPosition",
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

   private:
    rev::CANSparkMax m_wristMotor{CANSparkMaxConstants::kWristRotationMotorID,
                                  rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxPIDController m_pid = m_wristMotor.GetPIDController();

    rev::SparkMaxAbsoluteEncoder m_encoder = m_wristMotor.GetAbsoluteEncoder(
        rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle);

    SingleAxisConfig m_config = {
        .type = BaseSingleAxisSubsystem::AxisType::Rotational,
        .pid = frc::ProfiledPIDController<units::degree>(
            1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::degree>::Constraints(
                1.75_deg_per_s, 0.75_deg_per_s_sq)),
        .minDistance = 0,
        .maxDistance = ArmConstants::kWristDegreeLimit,
        .distancePerRevolution = 360.0,
        .motorMultiplier = 1.0,
        .minLimitSwitchPort = ArmConstants::kWristLimitSwitchPort,
        .maxLimitSwitchPort = BaseSingleAxisSubsystem::UNUSED_DIO_PORT,
        .defaultMovementSpeed = ArmConstants::kWristHomingSpeed};

    frc::DigitalInput min{ArmConstants::kWristLimitSwitchPort};
};
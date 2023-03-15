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
        : BaseSingleAxisSubsystem(m_config, m_wristMotor, m_encoder, &min,
                                  nullptr, "ROTATE WRIST") {}

    void ResetEncoder() override {}

    units::degree_t GetCurrentPosition() override {
        auto position = m_encoder.GetPosition() * 360_deg;

        Logging::logToSmartDashboard("WristPosition",
                                     std::to_string(position.value()),
                                     Logging::Level::INFO);

        return position;
    }

   private:
    rev::CANSparkMax m_wristMotor{CANSparkMaxConstants::kWristRotationMotorID,
                                  rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxAbsoluteEncoder m_encoder = m_wristMotor.GetAbsoluteEncoder(
        rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle);

    SingleAxisConfig m_config = {
        BaseSingleAxisSubsystem::AxisType::Rotational,
        frc::ProfiledPIDController<units::degree>(
            1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::degree>::Constraints(
                1.75_deg_per_s, 0.75_deg_per_s_sq)),
        0_deg,
        ArmConstants::kWristDegreeLimit,
        360_deg,
        BaseSingleAxisSubsystem::ConfigConstants::MOTOR_DIRECTION_NORMAL,
        ArmConstants::kWristLimitSwitchPort,
        BaseSingleAxisSubsystem::UNUSED_DIO_PORT,
        ArmConstants::kWristHomingSpeed};

    frc::DigitalInput min{ArmConstants::kWristLimitSwitchPort};
};
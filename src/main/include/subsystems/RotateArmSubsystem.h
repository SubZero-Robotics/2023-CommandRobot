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
        : BaseSingleAxisSubsystem(m_config, m_leadRotationMotor, m_enc) {
        m_followRotationMotor.Follow(m_leadRotationMotor);
    }

    void ResetEncoder() override { m_enc.SetZeroOffset(0); }

    units::degree_t GetCurrentPosition() override {
        auto position = m_enc.GetPosition() * 360_deg;

        Logging::logToSmartDashboard("RotatePose", std::to_string(position.value()),
                                     Logging::Level::INFO);

        return position;
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

    rev::SparkMaxAbsoluteEncoder m_enc = m_leadRotationMotor.GetAbsoluteEncoder(
        rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle);

    SingleAxisConfig m_config = {
        BaseSingleAxisSubsystem::AxisType::Rotational,
        frc::ProfiledPIDController<units::degree>(
            1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::angle::degree>::Constraints(
                1.75_deg_per_s, 0.75_deg_per_s_sq)),
        ArmConstants::kRotationHomeDegree,
        ArmConstants::kRotationMaxDegree,
        360_deg,
        BaseSingleAxisSubsystem::ConfigConstants::MOTOR_DIRECTION_NORMAL,
        ArmConstants::kRotationLimitSwitchHomePort,
        ArmConstants::kRotationLimitSwitchMaxPort,
        ArmConstants::kRotationHomingSpeed};
};
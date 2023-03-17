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
                                  &max, "ARM") {
        m_followRotationMotor.Follow(m_leadRotationMotor);
        // https://docs.wpilib.org/en/stable/docs/software/advanced-controls/introduction/tuning-vertical-arm.html
        _config = m_config;
        m_enc.SetPositionConversionFactor(_config.distancePerRevolution);
    }

    // Rotate arm has zero offset set in SparkMax
    void ResetEncoder() override {}

    double GetCurrentPosition() override {
        // "Home" is at 60 deg relative to ground
        auto position = m_enc.GetPosition() + _config.minDistance;

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
        .pid = frc::ProfiledPIDController<units::degree>(
            ArmConstants::kArmRotationSetP, ArmConstants::kArmRotationSetI, ArmConstants::kArmRotationSetD,
            frc::TrapezoidProfile<units::angle::degree>::Constraints(
                1.75_deg_per_s, 0.75_deg_per_s_sq)),
        .minDistance = ArmConstants::kRotationHomeDegree,
        .maxDistance = ArmConstants::kRotationMaxDegree,
        .distancePerRevolution = 360.0,
        .stepSize = ArmConstants::kArmStepSize,
        .motorMultiplier = -1.0,
        .minLimitSwitchPort = ArmConstants::kRotationLimitSwitchHomePort,
        .maxLimitSwitchPort = ArmConstants::kRotationLimitSwitchMaxPort,
        .defaultMovementSpeed = ArmConstants::kRotationHomingSpeed};

    frc::DigitalInput min{ArmConstants::kRotationLimitSwitchHomePort};
    frc::DigitalInput max{ArmConstants::kRotationLimitSwitchMaxPort};
};
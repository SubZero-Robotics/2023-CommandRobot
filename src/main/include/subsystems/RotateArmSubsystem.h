#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"

class RotateArmSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxAbsoluteEncoder> {
   public:
    RotateArmSubsystem();

    // Rotate arm has zero offset set in SparkMax
    void ResetEncoder() override;

    double GetCurrentPosition() override;

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
        .pidResultMultiplier = -0.5,
        .minLimitSwitchPort = ArmConstants::kRotationLimitSwitchHomePort,
        .maxLimitSwitchPort = ArmConstants::kRotationLimitSwitchMaxPort,
        .defaultMovementSpeed = ArmConstants::kRotationHomingSpeed};

    frc::DigitalInput min{ArmConstants::kRotationLimitSwitchHomePort};
    frc::DigitalInput max{ArmConstants::kRotationLimitSwitchMaxPort};
};
#include "subsystems/RotateArmSubsystem.h"

RotateArmSubsystem::RotateArmSubsystem(): 
 BaseSingleAxisSubsystem(m_config, m_leadRotationMotor) {
    // Implementation of subsystem constructor goes here.

    // m_leadRotationMotor.EnableSoftLimit(
    //     rev::CANSparkMax::SoftLimitDirection::kForward, true);
    // m_leadRotationMotor.EnableSoftLimit(
    //     rev::CANSparkMax::SoftLimitDirection::kReverse, true);

    // // Default unit is number of rotations
    // m_leadRotationMotor.SetSoftLimit(
    //     rev::CANSparkMax::SoftLimitDirection::kForward,
    //     ArmConstants::kForwardRotationsSoftLimit);
    // m_leadRotationMotor.SetSoftLimit(
    //     rev::CANSparkMax::SoftLimitDirection::kReverse,
    //     ArmConstants::kReverseRotationsSoftLimit);

    

    m_followRotationMotor.Follow(m_leadRotationMotor);
}

void RotateArmSubsystem::Periodic() {
    // todo: update angle of arm
    // todo: check limit switch
    // todo: log angle to shuffleboard
}

void RotateArmSubsystem::SimulationPeriodic() {}

void RotateArmSubsystem::PercentOutput(double output) {
    m_leadRotationMotor.Set(output);
}
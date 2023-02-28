#include "subsystems/RotateArmSubsystem.h"

RotateArmSubsystem::RotateArmSubsystem() {
    // Implementation of subsystem constructor goes here.

    double forwardRotationsSoftLimit = (kArmSoftLimitForwardDegrees * kArmTicksPerDegree) / kTicksPerMotorRotation;
    int reverseRotationsSoftLimit = 0;

    m_leadRotationMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);
    m_leadRotationMotor.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);

    // Default unit is number of rotations
    m_leadRotationMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, forwardRotationsSoftLimit);
    m_leadRotationMotor.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, reverseRotationsSoftLimit);

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
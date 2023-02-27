#include "subsystems/RotateArmSubsystem.h"

RotateArmSubsystem::RotateArmSubsystem() {
    // Implementation of subsystem constructor goes here.
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
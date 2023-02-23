#include "subsystems/EffectorSubsystem.h"

EffectorSubsystem::EffectorSubsystem() {
    // Implementation of subsystem constructor goes here.
    m_followRotationMotor.Follow(m_leadRotationMotor);
}

void EffectorSubsystem::Periodic() {
    // todo: update angle of arm
    // todo: check limit switch
    // todo: log angle to shuffleboard
}

void EffectorSubsystem::SimulationPeriodic() {
    
}

void EffectorSubsystem::PercentOutput(double output) {
    m_leadRotationMotor.Set(output);
}
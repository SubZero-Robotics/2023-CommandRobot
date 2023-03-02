// todo: map triggers to arm Brake and retraction distance
// todo: maintain state for limits
// todo: homing
// todo: encoder ticks to distance mapping

#include "commands/BrakeSet.h"

BrakeSet::BrakeSet(BrakeSubsystem* subsystem) : m_Brake{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Brake);
}

void BrakeSet::Execute() { m_Brake->SetBrakeMode(); 
isFinished = true;}
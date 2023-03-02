#include "commands/BrakeStop.h"

BrakeStop::BrakeStop(BrakeSubsystem* subsystem) : m_Brake{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Brake);
}

void BrakeStop::Execute() { m_Brake->UnsetBrakeMode(); isFinished = true; }
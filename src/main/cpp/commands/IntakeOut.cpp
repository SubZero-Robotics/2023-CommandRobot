#include "commands/IntakeOut.h"

IntakeOut::IntakeOut(IntakeSubsystem* subsystem)
    : m_Intake{subsystem}, isFinished{false} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Intake);
}

void IntakeOut::Execute() { m_Intake->Out(); }

bool IntakeOut::IsFinished() { return isFinished; }
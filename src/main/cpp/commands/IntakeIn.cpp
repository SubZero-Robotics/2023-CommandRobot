#include "commands/IntakeIn.h"

IntakeIn::IntakeIn(IntakeSubsystem* subsystem) : m_Intake{subsystem}, isFinished{false} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Intake);
}

void IntakeIn::Execute() {
    m_Intake->In();
}

bool IntakeIn::IsFinished() {
    return isFinished;
}
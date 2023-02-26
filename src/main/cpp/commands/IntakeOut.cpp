#include "commands/IntakeOut.h"

IntakeOut::IntakeOut(IntakeSubsystem* subsystem)
    : m_Intake{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Intake);
}

void IntakeOut::Execute() {
    m_Intake->Out();
}
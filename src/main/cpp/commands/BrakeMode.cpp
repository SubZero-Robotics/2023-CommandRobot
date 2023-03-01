#include "commands/BrakeMode.h"

BrakeMode::BrakeMode(DriveSubsystem* subsystem) : m_drive{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_drive);
}

void BrakeMode::Execute() { m_drive->BrakeInit(); }
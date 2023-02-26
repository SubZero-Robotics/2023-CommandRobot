#include "commands/ExtenderStop.h"

ExtenderStop::ExtenderStop(ExtensionSubsystem* subsystem)
    : m_extension{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_extension);
    Execute();
}

void ExtenderStop::Execute() { m_extension->PercentOutput(0.0); }
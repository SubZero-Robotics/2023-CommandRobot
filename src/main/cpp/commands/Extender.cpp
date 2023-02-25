// todo: map triggers to arm extension and retraction distance
// todo: maintain state for limits
// todo: homing
// todo: encoder ticks to distance mapping

#include "commands/Extender.h"

Extender::Extender(ExtensionSubsystem* subsystem, std::function<double()> rotation)
    : m_extension{subsystem}, m_rotation{rotation} {
    // Register that this command requires the subsystem.
    AddRequirements(m_extension);
    Execute();
}

void Extender::Execute() {
    double rotation = m_rotation();

    m_extension->PercentOutput(rotation);
}
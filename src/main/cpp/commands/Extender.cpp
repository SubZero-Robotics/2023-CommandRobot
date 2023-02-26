// todo: map triggers to arm extension and retraction distance
// todo: maintain state for limits
// todo: homing
// todo: encoder ticks to distance mapping

#include "commands/Extender.h"

Extender::Extender(ExtensionSubsystem* subsystem, 
std::function<double()> outExtent,
std::function<double()> inExtent
)
    : m_extension{subsystem}, 
    m_outExtent{outExtent},
    m_inExtent{inExtent}
    
    {
    // Register that this command requires the subsystem.
    AddRequirements(m_extension);
    Execute();
}

void Extender::Execute() {
    double outExtent = m_outExtent();
    double inExtent = m_inExtent();
    auto rotation = outExtent >= inExtent ? outExtent : -inExtent;

    m_extension->PercentOutput(rotation);
}
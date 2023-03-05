// todo: map triggers to arm extension and retraction distance
// todo: maintain state for limits
// todo: homing
// todo: encoder ticks to distance mapping

#include "commands/Extender.h"

#include <frc/smartdashboard/SmartDashboard.h>

Extender::Extender(ExtensionSubsystem* subsystem,
                   std::function<double()> outExtent,
                   std::function<double()> inExtent)
    : m_extension{subsystem},
      m_outExtent{outExtent},
      m_inExtent{inExtent}

{
    // Register that this command requires the subsystem.
    AddRequirements(m_extension);
}

void Extender::Execute() {
    double outExtent = m_outExtent();
    double inExtent = m_inExtent();
    auto rotation = outExtent >= inExtent ? outExtent : -inExtent;

    // if (!m_extension->AtLimit() ||
    //     m_extension->GetExtenderDistanceIn() >= ArmConstants::kMaxArmDistanceIn) {
    //     m_extension->PercentOutput(0);
    //     return;
    // }

    if (m_extension->AtLimit()) {
        m_extension->ResetEncoder();
        if (rotation > 0){
            m_extension->PercentOutput(rotation);
        } else {
            m_extension->PercentOutput(0.0);
        }
    } else {
        m_extension->PercentOutput(rotation);
    }

    // if (m_extension->GetExtenderDistanceIn() >= ArmConstants::kMaxArmDistanceIn) {
    //     m_extension->Extend(0.2);
    // }

}
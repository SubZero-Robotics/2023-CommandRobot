#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "Constants.h"
#include "subsystems/ExtensionSubsystem.h"

class Extender : public frc2::CommandHelper<frc2::CommandBase, Extender> {
   public:
    /**
     * Creates a new Extender.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit Extender(ExtensionSubsystem* subsystem,
                      std::function<double()> outExtent,
                      std::function<double()> inExtent)
        : m_extension{subsystem}, m_outExtent{outExtent}, m_inExtent{inExtent} {
        // Register that this command requires the subsystem.
        AddRequirements(m_extension);
    }

    void Execute() override {
        double outExtent = m_outExtent();
        double inExtent = m_inExtent();
        auto rotation = outExtent >= inExtent ? outExtent : -inExtent;

        m_extension->JoystickMoveStep(rotation);
    }

   private:
    ExtensionSubsystem* m_extension;
    std::function<double()> m_outExtent;
    std::function<double()> m_inExtent;
};
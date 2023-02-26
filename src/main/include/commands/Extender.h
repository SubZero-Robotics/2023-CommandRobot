#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

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
                      std::function<double()> inExtent);

    void Execute() override;

   private:
    ExtensionSubsystem* m_extension;
    std::function<double()> m_outExtent;
    std::function<double()> m_inExtent;
};
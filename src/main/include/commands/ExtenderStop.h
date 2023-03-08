#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/ExtensionSubsystem.h"

class ExtenderStop
    : public frc2::CommandHelper<frc2::CommandBase, ExtenderStop> {
   public:
    /**
     * Creates a new Extender.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit ExtenderStop(ExtensionSubsystem* subsystem)
        : m_extension{subsystem} {
        // Register that this command requires the subsystem.
        AddRequirements(m_extension);
    }

    void Execute() override { m_extension->PercentOutput(0.0); }

   private:
    ExtensionSubsystem* m_extension;
};
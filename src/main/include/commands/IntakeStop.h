#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/IntakeSubsystem.h"

class IntakeStop
    : public frc2::CommandHelper<frc2::CommandBase, IntakeStop> {
   public:
    /**
     * Creates a new Extender.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit IntakeStop(IntakeSubsystem* subsystem) : m_intake(subsystem) {
        AddRequirements(subsystem);
    }

    void Execute() override { m_intake->Stop(); }

   private:
    IntakeSubsystem* m_intake;
};
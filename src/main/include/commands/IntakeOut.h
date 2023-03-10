#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/IntakeSubsystem.h"

class IntakeOut : public frc2::CommandHelper<frc2::CommandBase, IntakeOut> {
   public:
    /**
     * Creates a new Intake.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit IntakeOut(IntakeSubsystem* subsystem)
        : m_intake{subsystem}, isFinished{false} {
        // Register that this command requires the subsystem.
        AddRequirements(m_intake);
    }

    void Execute() override { m_intake->Out(); }

    bool IsFinished() override { return isFinished; }

   private:
    IntakeSubsystem* m_intake;
    bool isFinished;
};
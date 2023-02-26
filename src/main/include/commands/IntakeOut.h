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
    explicit IntakeOut(IntakeSubsystem* subsystem);

    void Execute() override;

   private:
    IntakeSubsystem* m_Intake;
};
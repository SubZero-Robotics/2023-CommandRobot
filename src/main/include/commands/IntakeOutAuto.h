#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/IntakeSubsystem.h"

class IntakeOutAuto : public frc2::CommandHelper<frc2::CommandBase, IntakeOutAuto> {
   public:
    /**
     * Creates a new Intake.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit IntakeOutAuto(IntakeSubsystem* subsystem)
        : m_intake{subsystem}, isFinished{false} {
        // Register that this command requires the subsystem.
        AddRequirements(m_intake);
    }

    void Execute() override { 
        m_intake->Out();
        ++loops;
        if (loops >= 50) isFinished = true;
     }

    bool IsFinished() override { return isFinished; }

    void End(bool interrupted) override {
        m_intake->Stop();
    }

   private:
    IntakeSubsystem* m_intake;
    bool isFinished;
    int loops = 0;
};
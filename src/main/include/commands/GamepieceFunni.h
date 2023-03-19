#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/Timer.h>

#include "subsystems/IntakeSubsystem.h"

class GamepieceFunni : public frc2::CommandHelper<frc2::CommandBase, GamepieceFunni> {
   public:
    /**
     * Creates a new Intake.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit GamepieceFunni(IntakeSubsystem* subsystem)
        : m_intake{subsystem}, isFinished{false} {
        // Register that this command requires the subsystem.
        AddRequirements(m_intake);
        m_timer.Start();
    }

    void Execute() override {
        if (m_timer.AdvanceIfElapsed(200_ms)) {
            state = 1 - state;

            if (state) {
                m_intake->In();
            } else {
                m_intake->Out();
            }
        }
    }

    bool IsFinished() override { return isFinished; }

    void End(bool interrupted) override {
        m_intake->Stop();
    }

   private:
    IntakeSubsystem* m_intake;
    frc::Timer m_timer;
    uint8_t state = 0;
    bool isFinished;
};
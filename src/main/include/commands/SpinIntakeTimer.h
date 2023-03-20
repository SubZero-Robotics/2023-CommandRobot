#pragma once

#include <frc/Timer.h>
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/IntakeSubsystem.h"

class SpinIntakeTimer
    : public frc2::CommandHelper<frc2::CommandBase, SpinIntakeTimer> {
   public:
    /**
     * Creates a new Intake.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit SpinIntakeTimer(IntakeSubsystem* subsystem,
                             units::time::second_t seconds, bool intaking)
        : m_intake{subsystem},
          isFinished{false},
          m_intaking{intaking},
          m_seconds{seconds} {
        // Register that this command requires the subsystem.
        AddRequirements(m_intake);
        m_timer.Start();
    }

    void Execute() override {
        if (m_timer.AdvanceIfElapsed(m_seconds)) {
            if (m_intaking) {
                m_intake->In();
            } else {
                m_intake->Out();
            }
        }
    }

    bool IsFinished() override { return isFinished; }

    void End(bool interrupted) override { m_intake->Stop(); }

   private:
    IntakeSubsystem* m_intake;
    frc::Timer m_timer;
    bool isFinished;
    units::time::second_t m_seconds;
    bool m_intaking;
};
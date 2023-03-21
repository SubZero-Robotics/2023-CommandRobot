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
                             units::time::second_t duration, bool isIntakingIn)
        : m_intake{subsystem},
          m_isFinished{false},
          m_duration{duration},
          m_isIntaking{isIntakingIn} {
        // Register that this command requires the subsystem.
        AddRequirements(m_intake);
    }

    void Initialize() override {
        m_timer.Restart();
    }

    void Execute() override {
        if (m_isIntaking) {
            m_intake->In();
        } else {
            m_intake->Out();
        }

        if (m_timer.HasElapsed(m_duration)) {
            m_isFinished = true;
        }
    }

    bool IsFinished() override { return m_isFinished; }

    void End(bool interrupted) override { m_intake->Stop(); }

   private:
    IntakeSubsystem* m_intake;
    frc::Timer m_timer;
    bool m_isFinished;
    units::time::second_t m_duration;
    bool m_isIntaking;
};
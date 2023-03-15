#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/BrakeSubsystem.h"

class BrakeStop : public frc2::CommandHelper<frc2::CommandBase, BrakeStop> {
   public:
    /**
     * Creates a new Brake.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit BrakeStop(BrakeSubsystem* subsystem) : m_brake{subsystem} {
        // Register that this command requires the subsystem.
        AddRequirements(m_brake);
    }

    void Execute() override {
        m_brake->UnsetBrakeMode();
        isFinished = true;
    }

    bool IsFinished() override { return isFinished; }

   private:
    BrakeSubsystem* m_brake;
    bool isFinished = false;
};
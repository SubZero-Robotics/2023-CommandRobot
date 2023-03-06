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
    explicit BrakeStop(BrakeSubsystem* subsystem);

    void Execute() override;

    bool IsFinished() override { return isFinished; }

   private:
    BrakeSubsystem* m_Brake;
    bool isFinished = false;
};
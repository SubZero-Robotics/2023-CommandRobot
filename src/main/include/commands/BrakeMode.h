#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/DriveSubsystem.h"

class BrakeMode : public frc2::CommandHelper<frc2::CommandBase, BrakeMode> {
   public:
    /**
     * Creates a new RotateWrist.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit BrakeMode(DriveSubsystem* subsystem);

    void Execute() override;

    bool IsFinished() override {
        return true;
    }

   private:
    DriveSubsystem* m_drive;
};
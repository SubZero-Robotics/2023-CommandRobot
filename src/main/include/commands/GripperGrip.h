#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/GripperSubsystem.h"

class Gripper
    : public frc2::CommandHelper<frc2::CommandBase, Gripper> {
   public:
    /**
     * Creates a new Gripper.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit Gripper(GripperSubsystem* subsystem);

    void Execute() override;

   private:
    GripperSubsystem* m_Gripper;
};
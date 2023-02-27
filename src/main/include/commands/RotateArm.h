#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/RotateArmSubsystem.h"

class RotateArm : public frc2::CommandHelper<frc2::CommandBase, RotateArm> {
   public:
    /**
     * Creates a new RotateArm.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit RotateArm(RotateArmSubsystem* subsystem,
                       std::function<double()> m_rotation);

    void Execute() override;

   private:
    RotateArmSubsystem* m_effector;
    std::function<double()> m_rotation;
};
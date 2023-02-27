#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/WristSubsystem.h"

class RotateWrist : public frc2::CommandHelper<frc2::CommandBase, RotateWrist> {
   public:
    /**
     * Creates a new RotateWrist.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit RotateWrist(WristSubsystem* subsystem,
                       std::function<double()> m_rotation);

    void Execute() override;

   private:
    WristSubsystem* m_effector;
    std::function<double()> m_rotation;
};
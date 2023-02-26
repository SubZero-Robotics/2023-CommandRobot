#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/EffectorSubsystem.h"

class MoveArm : public frc2::CommandHelper<frc2::CommandBase, MoveArm> {
   public:
    /**
     * Creates a new MoveArm.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit MoveArm(EffectorSubsystem* subsystem,
                     std::function<double()> m_rotation);

    void Execute() override;

   private:
    EffectorSubsystem* m_effector;
    std::function<double()> m_rotation;
};
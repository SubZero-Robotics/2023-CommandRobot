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
                       std::function<double()> rotation)
        : m_effector{subsystem}, m_rotation{rotation} {
        // Register that this command requires the subsystem.
        AddRequirements(m_effector);
    }

    void Execute() override {
        double rotation = m_rotation();

        Logging::logToStdOut("arm rotate", std::to_string(rotation), Logging::Level::VERBOSE);

        if (abs(rotation) < kDeadzone) rotation = 0.0;

        m_effector->RunMotorExternal(rotation);
    }

   private:
    RotateArmSubsystem* m_effector;
    std::function<double()> m_rotation;
};
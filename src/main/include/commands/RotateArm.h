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

        if (abs(rotation) < kDeadzone) rotation = 0.0;

        if (m_effector->AtHome()) {
            if (rotation < 0) {
                m_effector->ResetEncoder();
                m_effector->PercentOutput(rotation);
            } else {
                m_effector->PercentOutput(0.0);
            }
        } else if (m_effector->AtMax()) {
            if (rotation > 0) {
                m_effector->PercentOutput(rotation);
            } else {
                m_effector->PercentOutput(0.0);
            }
        } else {
            m_effector->PercentOutput(rotation);
        }
    }

   private:
    RotateArmSubsystem* m_effector;
    std::function<double()> m_rotation;
};
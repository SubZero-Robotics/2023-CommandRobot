#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "Constants.h"
#include "subsystems/RotateArmSubsystem.h"

class RotateArmHome
    : public frc2::CommandHelper<frc2::CommandBase, RotateArmHome> {
   public:
    /**
     * Creates a new Extender.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit RotateArmHome(RotateArmSubsystem* subsystem)
        : m_rotation(subsystem), isFinished(false){};

    void Execute() override {
        if (!m_rotation->AtHome()) {
            m_rotation->RunMotorHoming(ArmConstants::kRotationHomingSpeed);
        } else {
            m_rotation->RunMotorHoming(0);
            m_rotation->ResetEncoder();
            isFinished = true;
        }
    }

    bool IsFinished() override { return isFinished; }

   private:
    RotateArmSubsystem* m_rotation;
    bool isFinished;
};
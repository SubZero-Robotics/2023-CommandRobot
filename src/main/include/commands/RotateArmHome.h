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
        : m_rotation(subsystem){};

    void Execute() override {
        while (!m_rotation->AtLimit()) {
            m_rotation->RunMotorHoming(.1);
        }

        m_rotation->RunMotorHoming(0);
        m_rotation->ResetEncoder();
    }

   private:
    RotateArmSubsystem* m_rotation;
};
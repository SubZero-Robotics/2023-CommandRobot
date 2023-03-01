#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "Constants.h"
#include "subsystems/WristSubsystem.h"

class WristHome
    : public frc2::CommandHelper<frc2::CommandBase, WristHome> {
   public:
    /**
     * Creates a new Wrist.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit WristHome(WristSubsystem* subsystem)
        : m_wristMotor(subsystem){};

    void Execute() override {
        while (!m_wristMotor->AtLimit()) {
            m_wristMotor->RunMotorHoming(ArmConstants::kExtenderHomingSpeed);
        }

        m_wristMotor->RunMotorHoming(0);
        m_wristMotor->ResetWristEncoder();
    }

   private:
    WristSubsystem* m_wristMotor;
};
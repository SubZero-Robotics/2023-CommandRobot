#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "Constants.h"
#include "subsystems/ExtensionSubsystem.h"

class ExtenderHome
    : public frc2::CommandHelper<frc2::CommandBase, ExtenderHome> {
   public:
    /**
     * Creates a new Extender.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit ExtenderHome(ExtensionSubsystem* subsystem)
        : m_extension(subsystem), isFinished(false) {}

    void Execute() override {
        if (!m_extension->AtLimit()) {
            m_extension->RunMotorHoming(ArmConstants::kExtenderHomingSpeed);
        } else {
            m_extension->RunMotorHoming(0);
            m_extension->ResetEncoder();
            isFinished = true;
        }
    }

    bool IsFinished() override { return isFinished; }

   private:
    ExtensionSubsystem* m_extension;
    bool isFinished;
};
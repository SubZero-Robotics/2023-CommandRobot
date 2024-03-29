// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/LEDControllerSubsystem.h"

/**
 * An example command that uses an example subsystem.
 *
 * <p>Note that this extends CommandHelper, rather extending CommandBase
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class LEDToggle : public frc2::CommandHelper<frc2::CommandBase, LEDToggle> {
   public:
    /**
     * Creates a new LEDYellow.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit LEDToggle(LEDControllerSubsystem* subsystem)
        : m_subsystem(subsystem) {
        AddRequirements(subsystem);
    }

    void Execute() override {
        if (m_subsystem->getCurrentColor() ==
            LEDControllerSubsystem::Colors::Purple) {
            m_subsystem->setColor(LEDControllerSubsystem::Colors::Yellow);
            m_subsystem->setPattern(LEDControllerSubsystem::PatternType::SetAll,
                                    true);
        } else {
            m_subsystem->setColor(LEDControllerSubsystem::Colors::Purple);
            m_subsystem->setPattern(LEDControllerSubsystem::PatternType::SetAll,
                                    true);
        }

        isFinished = true;
    }

    bool IsFinished() override { return isFinished; }

   private:
    LEDControllerSubsystem* m_subsystem;
    bool isFinished = false;
};
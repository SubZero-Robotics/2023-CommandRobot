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
class LEDYellow : public frc2::CommandHelper<frc2::CommandBase, LEDYellow> {
   public:
    /**
     * Creates a new LEDYellow.
     *
     * @param subsystem The subsystem used by this command.
     */
    explicit LEDYellow(LEDControllerSubsystem* subsystem)
        : m_subsystem(subsystem) {}

    void Execute() override {
        m_subsystem->setColor(255, 255, 0);
        m_subsystem->setPattern(LEDControllerSubsystem::PatternType::SetAll);
    }

   private:
    LEDControllerSubsystem* m_subsystem;
};

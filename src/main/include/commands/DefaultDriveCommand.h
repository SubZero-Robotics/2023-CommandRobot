/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include <cmath>

#include "Constants.h"
#include "subsystems/DriveSubsystem.h"

/**
 * A command to drive the robot with joystick input passed in through lambdas.
 * Written explicitly for pedagogical purposes - actual code should inline a
 * command this simple with RunCommand.
 *
 * @see RunCommand
 */
class DefaultDrive
    : public frc2::CommandHelper<frc2::CommandBase, DefaultDrive> {
   public:
    /**
     * Creates a new DefaultDrive.
     *
     * @param subsystem The drive subsystem this command will run on.
     * @param forward The control input for driving forwards/backwards
     * @param rotation The control input for turning
     */
    DefaultDrive(DriveSubsystem* subsystem, std::function<double()> forward,
                 std::function<double()> rotation,
                 std::function<bool()> precisionModeModifier)
        : m_drive{subsystem},
          m_forward{forward},
          m_rotation{rotation},
          m_precisionModeModifier(precisionModeModifier) {
        AddRequirements({subsystem});
    }

    void Execute() override {
        // Apply stick deadzone
        double XboxX = m_rotation();
        if (abs(XboxX) < kDeadzone) XboxX = 0.0;
        double XboxY = m_forward();
        if (abs(XboxY) < kDeadzone) XboxY = 0.0;

        // drive it
        if (m_precisionModeModifier()) {
            XboxX *= DriveConstants::kPrecisionModeXCoEff;
            XboxY *= DriveConstants::kPrecisionModeYCoEff;
        }
        m_drive->ArcadeDrive(XboxY, XboxX);
    }

   private:
    DriveSubsystem* m_drive;
    std::function<double()> m_forward;
    std::function<double()> m_rotation;
    std::function<bool()> m_precisionModeModifier;
};
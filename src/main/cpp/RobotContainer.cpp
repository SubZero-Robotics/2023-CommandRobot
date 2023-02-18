// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/button/Trigger.h>

#include "commands/Autos.h"
#include "commands/DefaultDrive.h"
#include "commands/ExampleCommand.h"

RobotContainer::RobotContainer() {
    // Initialize all of your commands and subsystems here

    // Configure the button bindings
    ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
    // Default drive command.  This will be run in teleop and when no other
    // command is running.
    m_drive.SetDefaultCommand(DefaultDrive(
        &m_drive, [this] { return Xbox.GetLeftY(); },
        [this] { return Xbox.GetLeftX(); }));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    return autos::StraightBack(&m_drive);
}

// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <iostream>

#include "commands/Autos.h"
#include "commands/DefaultDrive.h"
#include "commands/ExampleCommand.h"
#include "commands/MoveArm.h"
#include "commands/Extender.h"
#include "commands/ExtenderStop.h"

RobotContainer::RobotContainer() {
    // Initialize all of your commands and subsystems here
    drive = std::make_unique<DriveSubsystem>(RightLead, RightFollow, LeftLead,
                                             LeftFollow);
    m_drive = drive.get();

    // Configure the button bindings
    ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
    // Default drive command.  This will be run in teleop and when no other
    // command is running.
    m_drive->SetDefaultCommand(DefaultDrive(
        m_drive, [this] { return Xbox.GetLeftY(); },
        [this] { return Xbox.GetLeftX(); }));

    // TODO: bind buttons for calling commands

    m_effector.SetDefaultCommand(MoveArm(
        &m_effector, [this] { return Xbox.GetRightY(); }
    ));

    m_extender.SetDefaultCommand(ExtenderStop(&m_extender));

    Xbox.LeftBumper().OnTrue(Extender(
        &m_extender, [this] { return 1.0; }
    ).ToPtr());

    Xbox.RightBumper().OnTrue(Extender(
        &m_extender, [this] { return -1.0; }
    ).ToPtr());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    return autos::StraightBack(m_drive);
}

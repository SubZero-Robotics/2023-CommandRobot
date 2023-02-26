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
#include "commands/GripperGrip.h"
#include "commands/GripperStop.h"
#include "commands/IntakeIn.h"
#include "commands/IntakeOut.h"


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
        m_drive, [this] { return DriverXbox.GetLeftY(); },
        [this] { return -DriverXbox.GetLeftX() * 0.70; }));

    // TODO: bind buttons for calling commands

    m_effector.SetDefaultCommand(MoveArm(
        &m_effector, [this] { return ArmXbox.GetLeftY(); }
    ));

    m_extender.SetDefaultCommand(Extender(&m_extender, [this] { return ArmXbox.GetLeftY();}));

    ArmXbox.A().OnTrue(GripperStop(
        &m_gripper
    ).ToPtr());

    ArmXbox.A().OnFalse(Gripper(
        &m_gripper
    ).ToPtr());

    ArmXbox.X().OnTrue(IntakeOut(
        &m_intake
    ).ToPtr());

    ArmXbox.X().OnFalse(IntakeIn(
        &m_intake
    ).ToPtr());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    return autos::StraightBack(m_drive);
}

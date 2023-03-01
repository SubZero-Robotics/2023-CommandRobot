// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <iostream>

#include "commands/DefaultDrive.h"
#include "commands/ExampleCommand.h"
#include "commands/Extender.h"
#include "commands/ExtenderHome.h"
#include "commands/ExtenderStop.h"
#include "commands/GripperGrip.h"
#include "commands/GripperStop.h"
#include "commands/IntakeIn.h"
#include "commands/IntakeOut.h"
#include "commands/LEDPurple.h"
#include "commands/LEDYellow.h"
#include "commands/RotateArm.h"
#include "commands/RotateWrist.h"
#include "commands/RotateArmHome.h"

RobotContainer::RobotContainer() {
    // Initialize all of your commands and subsystems here
    drive = std::make_unique<DriveSubsystem>(RightLead, RightFollow, LeftLead,
                                             LeftFollow);
    m_drive = drive.get();

    // Configure the button bindings
    ConfigureBindings();

    m_chooser.SetDefaultOption("StraightBack", m_straightback.get());
    m_chooser.AddOption("Curved Path", m_testauto.get());

    frc::SmartDashboard::PutData(&m_chooser);

    m_leds.setOn();
}

void RobotContainer::ConfigureBindings() {
    // Default drive command.  This will be run in teleop and when no other
    // command is running.
    m_drive->SetDefaultCommand(DefaultDrive(
        m_drive, [this] { return DriverXbox.GetLeftY(); },
        [this] {
            return -DriverXbox.GetLeftX() * DriveConstants::kCurbRotation;
        }));

    // TODO: bind buttons for calling commands

    m_effector.SetDefaultCommand(
        RotateArm(&m_effector, [this] { return ArmXbox.GetLeftY(); }));

    m_wrist.SetDefaultCommand(
        RotateWrist(&m_wrist, [this] { return ArmXbox.GetRightY(); }));

    m_extender.SetDefaultCommand(Extender(
        &m_extender, [this] { return ArmXbox.GetRightTriggerAxis(); },
        [this] { return ArmXbox.GetLeftTriggerAxis(); }));

    DriverXbox.Y().ToggleOnTrue(RotateArmHome(&m_effector).ToPtr());

    DriverXbox.X().ToggleOnTrue(ExtenderHome(&m_extender).ToPtr());

    ArmXbox.A().OnTrue(IntakeOut(&m_intake).ToPtr());

    ArmXbox.B().OnFalse(IntakeIn(&m_intake).ToPtr());

    ArmXbox.LeftBumper().ToggleOnTrue(LEDYellow(&m_leds).ToPtr());

    ArmXbox.RightBumper().ToggleOnTrue(LEDPurple(&m_leds).ToPtr());
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    return m_chooser.GetSelected();
}

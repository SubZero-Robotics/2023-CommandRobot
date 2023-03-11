// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <iostream>

#include "commands/BrakeSetCommand.h"
#include "commands/BrakeStopCommand.h"
#include "commands/DefaultDriveCommand.h"
#include "commands/ExampleCommand.h"
#include "commands/ExtenderCommand.h"
#include "commands/ExtenderStopCommand.h"
#include "commands/IntakeInCommand.h"
#include "commands/IntakeOutCommand.h"
#include "commands/IntakeStopCommand.h"
#include "commands/LEDToggleCommand.h"
#include "commands/RotateArmCommand.h"
#include "commands/RotateWristCommand.h"

RobotContainer::RobotContainer() {
    // Initialize all of your commands and subsystems here
    drive = std::make_unique<DriveSubsystem>(RightLead, RightFollow, LeftLead,
                                             LeftFollow);
    m_drive = drive.get();

    // Configure the button bindings
    ConfigureBindings();

    m_chooser.SetDefaultOption("StraightBack", m_straightback.get());
    m_chooser.SetDefaultOption("DoesNothing", m_nothing.get());

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
        },
        [this] { return DriverXbox.GetBButton(); }));

    m_effector.SetDefaultCommand(
        RotateArm(&m_effector, [this] { return ArmXbox.GetLeftY(); }));

    m_wrist.SetDefaultCommand(
        RotateWrist(&m_wrist, [this] { return -ArmXbox.GetRightY(); }));

    m_extender.SetDefaultCommand(Extender(
        &m_extender, [this] { return ArmXbox.GetLeftTriggerAxis(); },
        [this] { return ArmXbox.GetRightTriggerAxis(); }));

    m_intake.SetDefaultCommand(IntakeStop(&m_intake).ToPtr());

    DriverXbox.Y().OnTrue(std::move(m_effector.GetHomeCommand()));

    DriverXbox.X().OnTrue(std::move(m_extender.GetHomeCommand()));

    DriverXbox.A().OnTrue(std::move(m_wrist.GetHomeCommand()));

    ArmXbox.LeftBumper().WhileTrue(IntakeOut(&m_intake).ToPtr());

    ArmXbox.RightBumper().WhileTrue(IntakeIn(&m_intake).ToPtr());

    DriverXbox.RightBumper().OnTrue(LEDToggle(&m_leds).ToPtr());

    DriverXbox.X().OnTrue(BrakeSet(&m_Brake).ToPtr());

    DriverXbox.Y().OnTrue(BrakeStop(&m_Brake).ToPtr());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    // TODO: return correct auto
    m_Brake.Set();
    return autos::StraightBack(m_drive);
}

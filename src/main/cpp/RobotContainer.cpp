// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <iostream>

#include "commands/BrakeSet.h"
#include "commands/BrakeStop.h"
#include "commands/DefaultDrive.h"
#include "commands/ExampleCommand.h"
#include "commands/Extender.h"
#include "commands/ExtenderHome.h"
#include "commands/ExtenderStop.h"
#include "commands/IntakeIn.h"
#include "commands/IntakeOut.h"
#include "commands/IntakeStop.h"
#include "commands/LEDToggle.h"
#include "commands/RotateArm.h"
#include "commands/RotateArmHome.h"
#include "commands/RotateWrist.h"
#include "commands/WristHome.h"

RobotContainer::RobotContainer() {
    // Initialize all of your commands and subsystems here
    drive = std::make_unique<DriveSubsystem>(RightLead, RightFollow, LeftLead,
                                             LeftFollow);
    m_drive = drive.get();

    // Configure the button bindings
    ConfigureBindings();

    m_chooser.SetDefaultOption("StraightBack", m_straightback.get());
    m_chooser.AddOption("DoesNothing", m_nothing.get());
    //m_chooser.AddOption("Curved Path", m_curves.get());

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

    // TODO: bind buttons for calling commands

    m_effector.SetDefaultCommand(
        RotateArm(&m_effector, [this] { return ArmXbox.GetLeftY(); }));

    m_wrist.SetDefaultCommand(
        RotateWrist(&m_wrist, [this] { return -ArmXbox.GetRightY(); }));

    m_extender.SetDefaultCommand(Extender(
        &m_extender, [this] { return ArmXbox.GetLeftTriggerAxis(); },
        [this] { return ArmXbox.GetRightTriggerAxis(); }));

    m_intake.SetDefaultCommand(IntakeStop(&m_intake).ToPtr());

    // TODO: add homing code
    // DriverXbox.Y().OnTrue(RotateArmHome(&m_effector).ToPtr());

    // DriverXbox.X().OnTrue(ExtenderHome(&m_extender).ToPtr());

    DriverXbox.A().OnTrue(WristHome(&m_wrist).ToPtr());

    ArmXbox.LeftBumper().WhileTrue(IntakeOut(&m_intake).ToPtr());

    ArmXbox.RightBumper().WhileTrue(IntakeIn(&m_intake).ToPtr());

    DriverXbox.RightBumper().OnTrue(LEDToggle(&m_leds).ToPtr());

    DriverXbox.X().OnTrue(BrakeSet(&m_Brake).ToPtr());

    DriverXbox.Y().OnTrue(BrakeStop(&m_Brake).ToPtr());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    // An example command will be run in autonomous
    // m_Brake.Unset();
    m_Brake.SetBrakeMode();
    //return autos::PlaceAndLeave(m_drive, &m_intake);
    return autos::PlaceAndBalance(m_drive, &m_intake);
}

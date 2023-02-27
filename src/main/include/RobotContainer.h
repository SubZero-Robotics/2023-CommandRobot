// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/Compressor.h>
#include <frc/XboxController.h>
#include <frc/controller/RamseteController.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc2/command/button/Trigger.h>
#include <pathplanner/lib/PathPlanner.h>
#include <pathplanner/lib/auto/RamseteAutoBuilder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <memory>

#include "Constants.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/ExampleSubsystem.h"
#include "subsystems/ExtensionSubsystem.h"
#include "subsystems/GripperSubsystem.h"
#include "subsystems/IntakeSubsystem.h"
#include "subsystems/LEDControllerSubsystem.h"
#include "subsystems/RotateArmSubsystem.h"
#include "subsystems/WristSubsystem.h"

/**
 * This class is where the bulk of the robot should be declared.  Since
 * Command-based is a "declarative" paradigm, very little robot logic should
 * actually be handled in the {@link Robot} periodic methods (other than the
 * scheduler calls).  Instead, the structure of the robot (including subsystems,
 * commands, and trigger mappings) should be declared here.
 */
class RobotContainer {
   public:
    RobotContainer();

    frc2::Command* GetAutonomousCommand();

   private:
    // Replace with CommandPS4Controller or CommandJoystick if needed
    frc2::CommandXboxController DriverXbox{0};
    // TODO: Second Xbox controller
    frc2::CommandXboxController ArmXbox{1};

    frc::Compressor phCompressor{4, frc::PneumaticsModuleType::REVPH};

    WPI_TalonFX RightLead{8};
    WPI_TalonFX RightFollow{7};

    WPI_TalonFX LeftLead{6};
    WPI_TalonFX LeftFollow{5};

    // The robot's subsystems are defined here...

    // This is the default wpilib example subsystem, it's only here until we
    // build an actual autonomous
    ExampleSubsystem m_subsystem;

    // Arm Subsystem
    RotateArmSubsystem m_effector;
    ExtensionSubsystem m_extender;
    GripperSubsystem m_gripper;
    IntakeSubsystem m_intake;
    LEDControllerSubsystem m_leds{kLEDCotrollerSlaveAddress};
    WristSubsystem m_wrist;

    // Drive subsystem from 2022. We should probably make cross season code
    // easier to reuse.
    std::unique_ptr<DriveSubsystem> drive;
    DriveSubsystem *m_drive;

    // Auto Builder
    std::unordered_map<std::string, std::shared_ptr<frc2::Command>> eventMap;
    pathplanner::RamseteAutoBuilder autoBuilder{
        [this]() { return m_drive->GetPose(); },
        [this](frc::Pose2d initPose) { m_drive->ResetOdometry(initPose); },
        frc::RamseteController(),
        (frc::DifferentialDriveKinematics)DriveConstants::kDriveKinematics,
        [this](units::meters_per_second_t left,
               units::meters_per_second_t right) {
            m_drive->TankDrive(left, right);
        },
        eventMap,
        {m_drive},
        true};

    frc2::CommandPtr m_straightback = autos::StraightBack(m_drive);
    frc2::CommandPtr m_testauto = autos::Test(autoBuilder, m_drive);

    frc::SendableChooser<frc2::Command*> m_chooser;

    void ConfigureBindings();
};

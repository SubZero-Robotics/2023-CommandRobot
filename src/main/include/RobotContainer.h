// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/Compressor.h>
#include <frc/XboxController.h>
#include <frc/controller/RamseteController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc2/command/button/Trigger.h>

#include <memory>

#include "Constants.h"
#include "commands/Autos.h"
#include "subsystems/BrakeSubsystem.h"
#include "subsystems/CompleteArmSubsystem.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/ExampleSubsystem.h"
#include "subsystems/ExtensionSubsystem.h"
#include "subsystems/IntakeSubsystem.h"
#include "subsystems/LEDControllerSubsystem.h"
#include "subsystems/RotateArmSubsystem.h"
#include "subsystems/VL53L1XSubsystem.h"
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
    frc2::CommandXboxController ArmXbox{1};

    WPI_TalonFX RightLead{DriveConstants::kRightLeadMotorID};
    WPI_TalonFX RightFollow{DriveConstants::kRightFollowMotorID};

    WPI_TalonFX LeftLead{DriveConstants::kLeftLeadMotorID};
    WPI_TalonFX LeftFollow{DriveConstants::kLeftFollowMotorID};

    // The robot's subsystems are defined here...

    // This is the default wpilib example subsystem, it's only here until we
    // build an actual autonomous
    ExampleSubsystem m_subsystem;

    DriveSubsystem drive{RightLead, RightFollow, LeftLead, LeftFollow};

    // Arm Subsystem
    std::unique_ptr<RotateArmSubsystem> m_effector;
    std::unique_ptr<ExtensionSubsystem> m_extender;
    BrakeSubsystem m_Brake{RightLead, LeftLead};
    LEDControllerSubsystem m_leds{kLEDCotrollerSlaveAddress,
                                  frc::I2C::Port::kMXP};
    IntakeSubsystem m_intake{&m_leds};
    std::unique_ptr<WristSubsystem> m_wrist;
    // std::unique_ptr<CompleteArmSubsystem> m_arm;
    VL53L1XController m_lidar{kLidarInputPin, kLidarValidationPin};

    // Drive subsystem from 2022. We should probably make cross season code
    // easier to reuse.
    DriveSubsystem* m_drive;

    frc2::CommandPtr m_straightback = autos::StraightBack(&drive, 60);
    frc2::CommandPtr m_nothing = autos::DoesNothing(&drive);
    frc2::CommandPtr m_placeandleave = autos::PlaceAndLeave(&drive, &m_intake);
    frc2::CommandPtr m_placeandbalance = autos::PlaceAndBalance(&drive, &m_intake);

    std::unique_ptr<CompleteArmSubsystem> m_arm;

    frc::SendableChooser<frc2::Command*> m_chooser;

    void ConfigureBindings();
};

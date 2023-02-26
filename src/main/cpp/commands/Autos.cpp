// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/Autos.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/Commands.h>
#include <frc2/command/FunctionalCommand.h>
#include <pathplanner/lib/auto/RamseteAutoBuilder.h>
#include <pathplanner/lib/PathPlanner.h>

using namespace AutoConstants;
using namespace pathplanner;

// This is taken from the hatchbotinlined wpilib example
frc2::CommandPtr autos::StraightBack(DriveSubsystem* m_drive) {
    return frc2::FunctionalCommand(
               // Reset encoders on command start
               [m_drive] { m_drive->ResetEncoders(); },
               // Drive forward while the command is executing
               [m_drive] {
                   m_drive->ArcadeDrive(AutoConstants::kAutoDriveSpeed, 0);
               },
               // Stop driving at the end of the command
               [m_drive](bool interrupted) { m_drive->ArcadeDrive(0, 0); },
               // End the command when the robot's driven distance exceeds the
               // desired value
               [m_drive] {
                   frc::SmartDashboard::PutNumber(
                       "Left Encoder",
                       m_drive->GetLeftEncoder());
                    frc::SmartDashboard::PutNumber(
                       "Right Encoder",
                       m_drive->GetRightEncoder());
                   return abs(m_drive->GetAverageEncoderDistance()) >=
                          AutoConstants::kAutoDriveDistanceInches / kMagicalAutoNumber;
               },
               // Requires the drive subsystem
               {m_drive})
        .ToPtr();
}

std::vector<PathPlannerTrajectory> pathGroup = PathPlanner::loadPathGroup("Test", {PathConstraints(4_mps, 3_mps_sq)});

frc2::CommandPtr autos::Test(pathplanner::RamseteAutoBuilder &autoBuilder, DriveSubsystem* m_drive) {
    return autoBuilder.fullAuto(pathGroup);
}
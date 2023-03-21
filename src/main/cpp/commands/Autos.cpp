// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/Autos.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/Commands.h>
#include <frc2/command/FunctionalCommand.h>

#include "commands/SpinIntakeTimer.h"
#include "subsystems/IntakeSubsystem.h"

using namespace AutoConstants;

// This is taken from the hatchbotinlined wpilib example
frc2::CommandPtr autos::StraightBack(DriveSubsystem* m_drive, double distance, double speed) {
    return frc2::FunctionalCommand(
               // Reset encoders on command start
               [m_drive] { m_drive->ResetEncoders(); },
               // Drive forward while the command is executing
               [m_drive, speed] {
                   m_drive->ArcadeDrive(speed, 0);
               },
               // Stop driving at the end of the command
               [m_drive](bool interrupted) { m_drive->ArcadeDrive(0, 0); },
               // End the command when the robot's driven distance exceeds the
               // desired value
               [m_drive, distance] {
                   frc::SmartDashboard::PutNumber("Left Encoder",
                                                  m_drive->GetLeftEncoder());
                   frc::SmartDashboard::PutNumber("Right Encoder",
                                                  m_drive->GetRightEncoder());
                   return abs(m_drive->GetAverageEncoderDistance()) >=
                          distance / kStraightBackDivisor;
               },
               // Requires the drive subsystem
               {m_drive})
        .ToPtr();
}

frc2::CommandPtr autos::DoesNothing(DriveSubsystem* m_drive) {
    return frc2::FunctionalCommand(
               [m_drive] { m_drive->ResetEncoders(); },
               // Drive forward while the command is executing
               [m_drive] { m_drive->ArcadeDrive(0, 0); },
               // Stop driving at the end of the command
               [m_drive](bool interrupted) { m_drive->ArcadeDrive(0, 0); },
               // End the command when the robot's driven distance exceeds the
               // desired value
               [m_drive] { return true; },
               // Requires the drive subsystem
               {m_drive})
        .ToPtr();
}

frc2::CommandPtr autos::PlaceAndLeave(DriveSubsystem* m_drive,
                                      IntakeSubsystem* m_intake) {
    return SpinIntakeTimer(m_intake, 1000_ms, false)
        .ToPtr()
        .AndThen(autos::StraightBack(m_drive, 155));
}

frc2::CommandPtr autos::PlaceAndBalance(DriveSubsystem* m_drive,
                                        IntakeSubsystem* m_intake) {
    return SpinIntakeTimer(m_intake, 1000_ms, false)
        .ToPtr()
        .AndThen(autos::StraightBack(m_drive, 97));
}
/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/autos/StraightBack.h"

#include <frc2/command/Commands.h>
#include <frc2/command/FunctionalCommand.h>

#include "commands/Autos.h"

frc2::CommandPtr autos::StraightBack(DriveSubsystem* drive) {
  return frc2::FunctionalCommand(
             // Reset encoders on command start
             [drive] { drive->ResetEncoders(); },
             // Drive forward while the command is executing
             [drive] { drive->ArcadeDrive(AutoConstants::kAutoDriveSpeed, 0); },
             // Stop driving at the end of the command
             [drive](bool interrupted) { drive->ArcadeDrive(0, 0); },
             // End the command when the robot's driven distance exceeds the
             // desired value
             [drive] {
               return drive->GetAverageEncoderDistance() >=
                      AutoConstants::kAutoDriveDistanceInches;
             },
             // Requires the drive subsystem
             {drive})
      .ToPtr();
}



void StraightBackRun::Initialize() {
  // An Top Auto Two Ball Trajectory
  frc::Trajectory StraightBack1;
   fs::path deployDirectoryuno = frc::filesystem::GetDeployDirectory();
   deployDirectoryuno = deployDirectoryuno / "pathplanner" / "generatedJSON" / "StraightBack.wpilib.json";
   StraightBack1 = frc::TrajectoryUtil::FromPathweaverJson(deployDirectoryuno.string());
   
  // Reset odometry to the starting pose of the trajectory.
  m_drive->ResetOdometry(StraightBack1.InitialPose());

  frc2::SequentialCommandGroup* myStraightBackAuto = new frc2::SequentialCommandGroup(
    frc2::InstantCommand([this] { m_drive->TankDriveVolts(0_V, 0_V); }, {} )
    );
  myStraightBackAuto->Schedule();
}

bool StraightBackRun::IsFinished() { return finished; }

void StraightBackRun::End(bool interrupted) {

}
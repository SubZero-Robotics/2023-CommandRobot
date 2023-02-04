/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/


#include "commands/autos/TestAuto.h"

#include <frc/controller/PIDController.h>
#include <frc/controller/RamseteController.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include <frc2/command/RamseteCommand.h>

#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/ParallelCommandGroup.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <subsystems/DriveSubsystem.h>

void TestAuto::Initialize() {
    frc::Trajectory StraightBack;
    fs::path deployDirectory = frc::filesystem::GetDeployDirectory();
    deployDirectory = deployDirectory / "pathplanner" / "generatedJSON" / "StraightBack.wpilib.json";
    StraightBack = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory.string());

    DriveSubsystem m_drive;
    m_drive.ResetOdometry(StraightBack.InitialPose());

}
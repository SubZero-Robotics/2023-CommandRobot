/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc2/command/CommandBase.h> 
#include <frc2/command/CommandHelper.h>
#include <frc/XboxController.h>

#include <frc/Filesystem.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <wpi/fs.h>

#include "subsystems/DriveSubsystem.h"

class StraightBackRun
    : public frc2::CommandHelper<frc2::CommandBase, StraightBackRun> {
 public:
  /**
   * Creates a new DriveTwoBallUnoTrajectory.
   *
   * takes no parameters: the trajectory is hardcoded in here
   * 
   */
  StraightBackRun(DriveSubsystem* dsubsystem);

  void Initialize() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

 private:
  DriveSubsystem* m_drive;
  bool finished = false;
  // The controller
  frc::XboxController Xbox{0};
};
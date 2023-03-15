// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <pathplanner/lib/PathPlanner.h>
#include <pathplanner/lib/auto/RamseteAutoBuilder.h>

#include "subsystems/DriveSubsystem.h"

namespace autos {
/**
 * Example static factory for an autonomous command.
 */
frc2::CommandPtr StraightBack(DriveSubsystem* m_drive);
frc2::CommandPtr Test(pathplanner::RamseteAutoBuilder& autoBuilder,
                      DriveSubsystem* m_drive);
frc2::CommandPtr DoesNothing(DriveSubsystem* m_drive);
}  // namespace autos

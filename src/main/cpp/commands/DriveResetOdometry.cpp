#include "commands/DriveResetOdometry.h"

#include <frc/geometry/Pose2d.h>

DriveResetOdometry::DriveResetOdometry(DriveSubsystem* subsystem,
                                       frc::Joystick* controller)
    : m_drive(subsystem), m_controller(controller) {
    AddRequirements({subsystem});
}

void DriveResetOdometry::Initialize() {
    frc::Pose2d currentRobotPose;  // is also zeroed by default
    m_drive->ResetOdometry(currentRobotPose);
}

void DriveResetOdometry::Execute() {}

void DriveResetOdometry::End(bool interrupted) {}

bool DriveResetOdometry::IsFinished() { return false; }